/**
 * \file test_database_datastore_delete.cpp
 *
 * \brief Test the vcdb_database_datastore_delete() method.
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
 * Test that we can begin a transaction and delete a value.
 */
TEST(datastore_delete, happy_path)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;
    const char* KEY = "test_key";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database and start a transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* preconditions */
    test_datastore_reset();
    ASSERT_FALSE(test_datastore_delete_called);

    /* delete a value. */
    char* key = strdup(KEY);
    size_t key_size = strlen(KEY) + 1;
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_datastore_delete(
            &transaction, &datastore, key, &key_size));

    /* postconditions */
    EXPECT_TRUE(test_datastore_delete_called);
    EXPECT_EQ(&transaction, test_datastore_delete_param_transaction);
    EXPECT_EQ(&datastore, test_datastore_delete_param_datastore);
    EXPECT_EQ(key, (const void*)test_datastore_delete_param_key);
    EXPECT_EQ(&key_size, test_datastore_delete_param_key_size);

    /* cleanup */
    free(key);
    dispose((disposable_t*)&transaction);
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that using delete with a rolled back transaction results in an error.
 */
TEST(datastore_delete, bad_transaction)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;
    const char* KEY = "test_key";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database and start a transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* roll back the transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, vcdb_transaction_rollback(&transaction));

    /* preconditions */
    test_datastore_reset();
    ASSERT_FALSE(test_datastore_delete_called);

    /* delete a value -- this should fail. */
    char* key = strdup(KEY);
    size_t key_size = strlen(key) + 1;
    ASSERT_EQ(VCDB_ERROR_BAD_TRANSACTION,
        vcdb_database_datastore_delete(
            &transaction, &datastore, key, &key_size));

    /* postconditions */
    EXPECT_FALSE(test_datastore_delete_called);

    /* cleanup */
    free(key);
    dispose((disposable_t*)&transaction);
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that datastore_delete returns an error for invalid parameters.
 */
TEST(datastore_delete, bad_params)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;
    const char* KEY = "test_key";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database and start a transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* delete a value. */
    char* key = strdup(KEY);
    size_t key_size = strlen(key) + 1;
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_delete(
            NULL, &datastore, key, &key_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_delete(
            &transaction, NULL, key, &key_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_delete(
            &transaction, &datastore, NULL, &key_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_delete(
            &transaction, &datastore, key, NULL));

    /* cleanup */
    free(key);
    dispose((disposable_t*)&transaction);
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}
