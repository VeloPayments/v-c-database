/**
 * \file test_transaction_rollback.cpp
 *
 * \brief Test the vcdb_transaction_rollback() method.
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
 * Test that we can roll back transactions
 */
TEST(transaction_rollback, happy_path)
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

    /* the begin transaction method should succeed. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* preconditions */
    ASSERT_FALSE(test_transaction_rollback_called);
    EXPECT_TRUE(transaction.in_transaction);

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
 * Test that rollback returns an error with a bad parameter.
 */
TEST(transaction_rollback, bad_param)
{
    /* register the test database engine. */
    register_test_database();

    /* roll back should return an error code. */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_transaction_rollback(NULL));
}
