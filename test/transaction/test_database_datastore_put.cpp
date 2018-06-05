/**
 * \file test_database_datastore_put.cpp
 *
 * \brief Test the vcdb_database_datastore_put() method.
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
 * Test that we can begin a transaction and put a value.
 */
TEST(datastore_put, happy_path)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;
    const char* KEY = "test_key";
    const char* VALUE = "test_value";

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
    ASSERT_FALSE(test_key_getter_called);
    ASSERT_FALSE(test_datastore_put_called);

    /* put a value. */
    test_value_t test_value;
    memset(&test_value, 0, sizeof(test_value));
    strcpy(test_value.test_key, KEY);
    strcpy(test_value.test_value, VALUE);
    size_t test_value_size = sizeof(test_value);
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_datastore_put(
            &transaction, &datastore, &test_value, &test_value_size));

    /* postconditions */
    EXPECT_TRUE(test_key_getter_called);
    EXPECT_EQ(&test_value, test_key_getter_param_value);
    EXPECT_NE(nullptr, test_key_getter_param_key);
    EXPECT_NE(nullptr, test_key_getter_param_key_size);
    EXPECT_TRUE(test_datastore_put_called);
    EXPECT_EQ(&transaction, test_datastore_put_param_transaction);
    EXPECT_EQ(&datastore, test_datastore_put_param_datastore);
    EXPECT_NE(nullptr, test_datastore_put_param_value);
    EXPECT_NE(nullptr, test_datastore_put_param_value_size);

    /* cleanup */
    dispose((disposable_t*)&transaction);
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that datastore_put returns an error for invalid parameters.
 */
TEST(datastore_put, bad_params)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_transaction_t transaction;
    const char* KEY = "test_key";
    const char* VALUE = "test_value";

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

    /* put a value. */
    test_value_t test_value;
    memset(&test_value, 0, sizeof(test_value));
    strcpy(test_value.test_key, KEY);
    strcpy(test_value.test_value, VALUE);
    size_t test_value_size = sizeof(test_value);
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_put(
            NULL, &datastore, &test_value, &test_value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_put(
            &transaction, NULL, &test_value, &test_value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_put(
            &transaction, &datastore, NULL, &test_value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_put(
            &transaction, &datastore, &test_value, NULL));

    /* cleanup */
    dispose((disposable_t*)&transaction);
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}
