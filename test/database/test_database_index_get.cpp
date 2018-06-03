/**
 * \file test_database_index_get.cpp
 *
 * \brief Test the vcdb_database_index_get() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/database.h>

#include "../test_database.h"
#include "../test_datastore.h"
#include "../test_index.h"

/**
 * Test that the index_get method calls the appropriate engine methods.
 */
TEST(database_index_get, e2e)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_index_t index;
    const char* key = "TESTKEY";
    size_t key_size = strlen(key);
    char value[1024];
    size_t value_size = sizeof(value);

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    test_datastore_reset();
    test_index_reset();
    ASSERT_FALSE(test_index_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;
    test_secondary_key_getter_called = false;

    /* call to vcdb_database_index_get should succeed. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the secondary key method should NOT have been called */
    EXPECT_FALSE(test_secondary_key_getter_called);

    /* the index_get method should have been called */
    EXPECT_TRUE(test_index_get_called);
    EXPECT_EQ(&database, test_index_get_param_database);
    EXPECT_EQ(&index, test_index_get_param_index);
    EXPECT_EQ(key, test_index_get_param_key);
    EXPECT_EQ(key_size, test_index_get_param_key_size);
    EXPECT_NE(nullptr, test_index_get_param_value);
    EXPECT_NE(nullptr, test_index_get_param_value_size);

    /* the serialization reader method should have been called. */
    EXPECT_TRUE(test_value_reader_called);
    EXPECT_EQ(test_index_get_param_value, test_value_reader_param_input);
    EXPECT_EQ(*test_index_get_param_value_size,
        test_value_reader_param_size);
    EXPECT_EQ(value, test_value_reader_param_value);

    /* clean up */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that the index_get method fails on invalid parameters.
 */
TEST(database_index_get, bad_params)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_index_t index;
    const char* key = "TESTKEY";
    size_t key_size = strlen(key);
    char value[1024];
    size_t value_size = sizeof(value);

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    test_datastore_reset();
    test_index_reset();
    ASSERT_FALSE(test_index_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;
    test_secondary_key_getter_called = false;

    /* call to vcdb_database_index_get should fail (bad parameter). */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_get(
            NULL, &index,
            (void*)key, key_size,
            (void*)value, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_get(
            &database, NULL,
            (void*)key, key_size,
            (void*)value, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_get(
            &database, &index,
            NULL, key_size,
            (void*)value, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, 0U,
            (void*)value, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, key_size,
            NULL, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, key_size,
            (void*)value, NULL));

    /* the secondary key method should NOT have been called */
    EXPECT_FALSE(test_secondary_key_getter_called);

    /* the index_get method should NOT have been called */
    EXPECT_FALSE(test_index_get_called);

    /* the serialization reader method should NOT have been called. */
    EXPECT_FALSE(test_value_reader_called);

    /* clean up */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that the index_get method fails when the value size is too small for the
 * value type.
 */
TEST(database_index_get, bad_value_size)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_index_t index;
    const char* key = "TESTKEY";
    size_t key_size = strlen(key);
    char value[1024];
    size_t value_size = 10;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    test_datastore_reset();
    test_index_reset();
    ASSERT_FALSE(test_index_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;
    test_secondary_key_getter_called = false;

    /* call to vcdb_database_index_get should fail (bad parameter). */
    ASSERT_EQ(VCDB_ERROR_WOULD_TRUNCATE,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the secondary key method should NOT have been called */
    EXPECT_FALSE(test_secondary_key_getter_called);

    /* the index_get method should NOT have been called */
    EXPECT_FALSE(test_index_get_called);

    /* the serialization reader method should NOT have been called. */
    EXPECT_FALSE(test_value_reader_called);

    /* clean up */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that the index_get method fails when the serialization method fails.
 */
TEST(database_index_get, bad_serialization)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    vcdb_index_t index;
    const char* key = "TESTKEY";
    size_t key_size = strlen(key);
    char value[1024];
    size_t value_size = sizeof(value);

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    test_datastore_reset();
    test_index_reset();
    ASSERT_FALSE(test_index_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = -12;
    test_secondary_key_getter_called = false;

    /* call to vcdb_database_index_get should fail (bad parameter). */
    ASSERT_EQ(-12,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the secondary key method should NOT have been called */
    EXPECT_FALSE(test_secondary_key_getter_called);

    /* the index_get method should have been called */
    EXPECT_TRUE(test_index_get_called);

    /* the serialization reader method should have been called. */
    EXPECT_TRUE(test_value_reader_called);

    /* clean up */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}
