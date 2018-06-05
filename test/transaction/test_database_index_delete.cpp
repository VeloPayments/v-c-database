/**
 * \file test_database_index_delete.cpp
 *
 * \brief Test the vcdb_database_index_delete() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/database.h>
#include <vcdb/datastore.h>
#include <vcdb/transaction.h>

#include "../test_database.h"
#include "../test_datastore.h"
#include "../test_index.h"

/**
 * Test that we can begin a transaction and delete a value.
 */
TEST(index_delete, happy_path)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_index_t index;
    vcdb_transaction_t transaction;
    const char* KEY = "test_key";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database and start a transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_index(&builder, &index));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* preconditions */
    test_index_reset();
    ASSERT_FALSE(test_index_delete_called);

    /* delete a value. */
    char* key = strdup(KEY);
    size_t key_size = strlen(KEY) + 1;
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_index_delete(
            &transaction, &index, key, &key_size));

    /* postconditions */
    EXPECT_TRUE(test_index_delete_called);
    EXPECT_EQ(&transaction, test_index_delete_param_transaction);
    EXPECT_EQ(&index, test_index_delete_param_index);
    EXPECT_EQ(key, (const void*)test_index_delete_param_key);
    EXPECT_EQ(&key_size, test_index_delete_param_key_size);

    /* cleanup */
    free(key);
    dispose((disposable_t*)&transaction);
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that using delete with a rolled back transaction results in an error.
 */
TEST(index_delete, bad_transaction)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_index_t index;
    vcdb_transaction_t transaction;
    const char* KEY = "test_key";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database and start a transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_index(&builder, &index));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* roll back the transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, vcdb_transaction_rollback(&transaction));

    /* preconditions */
    test_datastore_reset();
    test_index_reset();
    ASSERT_FALSE(test_index_delete_called);

    /* delete a value -- this should fail. */
    char* key = strdup(KEY);
    size_t key_size = strlen(key) + 1;
    ASSERT_EQ(VCDB_ERROR_BAD_TRANSACTION,
        vcdb_database_index_delete(
            &transaction, &index, key, &key_size));

    /* postconditions */
    EXPECT_FALSE(test_index_delete_called);

    /* cleanup */
    free(key);
    dispose((disposable_t*)&transaction);
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that index_delete returns an error for invalid parameters.
 */
TEST(index_delete, bad_params)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_index_t index;
    vcdb_transaction_t transaction;
    const char* KEY = "test_key";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database and start a transaction. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_index(&builder, &index));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_transaction_begin(&transaction, &database));

    /* delete a value. */
    char* key = strdup(KEY);
    size_t key_size = strlen(key) + 1;
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_delete(
            NULL, &index, key, &key_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_delete(
            &transaction, NULL, key, &key_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_delete(
            &transaction, &index, NULL, &key_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_delete(
            &transaction, &index, key, NULL));

    /* cleanup */
    free(key);
    dispose((disposable_t*)&transaction);
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}
