/**
 * \file test_transaction_begin.cpp
 *
 * \brief Test the vcdb_transaction_begin() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/database.h>
#include <vcdb/datastore.h>
#include <vcdb/transaction.h>

#include "../test_database.h"
#include "../test_datastore.h"

/**
 * Test that we can begin and roll back a transaction.
 */
TEST(transaction_begin, begin_rollback)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_transaction_begin_called);

    /* the begin transaction method should succeed. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* postconditions */
    EXPECT_TRUE(test_transaction_begin_called);
    EXPECT_EQ(&transaction, test_transaction_begin_param_transaction);
    EXPECT_EQ(&database, test_transaction_begin_param_database);
    EXPECT_TRUE(transaction.in_transaction);

    /* preconditions */
    ASSERT_FALSE(test_transaction_rollback_called);

    /* roll back the transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_rollback(&transaction));

    /* postconditions */
    EXPECT_TRUE(test_transaction_rollback_called);
    EXPECT_EQ(&transaction, test_transaction_rollback_param_transaction);
    EXPECT_FALSE(transaction.in_transaction);

    /* cleanup */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that an invalid parameter to transaction_begin ends in an error.
 */
TEST(transaction_begin, bad_param)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_transaction_begin_called);

    /* the begin transaction method should fail. */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_transaction_begin(NULL, &database));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_transaction_begin(&transaction, NULL));

    /* postconditions */
    EXPECT_FALSE(test_transaction_begin_called);
    EXPECT_FALSE(transaction.in_transaction);

    /* cleanup */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that an error creating a transaction in the engine bubbles up to the
 * begin method.
 */
TEST(transaction_begin, bad_engine)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_transaction_begin_called);
    test_transaction_begin_retval = -17;

    /* the begin transaction method should fail. */
    ASSERT_EQ(-17,
        vcdb_transaction_begin(&transaction, &database));

    /* postconditions */
    EXPECT_TRUE(test_transaction_begin_called);
    EXPECT_EQ(&transaction, test_transaction_begin_param_transaction);
    EXPECT_EQ(&database, test_transaction_begin_param_database);
    EXPECT_FALSE(transaction.in_transaction);

    /* cleanup */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that disposing a transaction is the same as rolling it back.
 */
TEST(transaction_begin, dispose_rollback)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_transaction_begin_called);

    /* the begin transaction method should succeed. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* postconditions */
    EXPECT_TRUE(test_transaction_begin_called);
    EXPECT_EQ(&transaction, test_transaction_begin_param_transaction);
    EXPECT_EQ(&database, test_transaction_begin_param_database);
    EXPECT_TRUE(transaction.in_transaction);

    /* preconditions */
    EXPECT_FALSE(test_transaction_rollback_called);
    EXPECT_TRUE(transaction.in_transaction);

    /* dispose the transaction. */
    dispose((disposable_t*)&transaction);

    /* postconditions */
    EXPECT_TRUE(test_transaction_rollback_called);
    EXPECT_FALSE(transaction.in_transaction);

    /* cleanup */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that if we roll back the transaction, we do not get a double-rollback in
 * dispose.
 */
TEST(transaction_begin, rollback_dispose)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_transaction_begin_called);

    /* the begin transaction method should succeed. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* postconditions */
    EXPECT_TRUE(test_transaction_begin_called);
    EXPECT_EQ(&transaction, test_transaction_begin_param_transaction);
    EXPECT_EQ(&database, test_transaction_begin_param_database);
    EXPECT_TRUE(transaction.in_transaction);

    /* preconditions */
    EXPECT_FALSE(test_transaction_rollback_called);
    EXPECT_TRUE(transaction.in_transaction);

    /* roll back the transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_rollback(&transaction));

    /* postconditions */
    EXPECT_TRUE(test_transaction_rollback_called);
    EXPECT_FALSE(transaction.in_transaction);

    /* preconditions */
    test_transaction_rollback_called = false;
    EXPECT_FALSE(transaction.in_transaction);

    /* dispose the transaction */
    dispose((disposable_t*)&transaction);

    /* postconditions */
    EXPECT_FALSE(test_transaction_rollback_called);
    EXPECT_FALSE(transaction.in_transaction);

    /* cleanup */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}
