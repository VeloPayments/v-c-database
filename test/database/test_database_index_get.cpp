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

/* forward decls */
static int test_datastore_init(vcdb_datastore_t*);
static void test_key_getter(const void* value, void* key, size_t* key_size);
static void test_2nd_key_getter(const void* value, void* key, size_t* key_size);
static int test_value_reader(const void* input, size_t size, void* value);
static int test_value_writer(const void* value, void* output, size_t* size);

/* method call accessors for test_value_reader */
static bool test_value_reader_called;
static const void* test_value_reader_param_input;
static size_t test_value_reader_param_size;
static void* test_value_reader_param_value;
static int test_value_reader_retval;

/* method call accessors for test_2nd_key_getter */
static bool test_2nd_key_getter_called;
static const void* test_2nd_key_getter_param_value;
static void* test_2nd_key_getter_param_key;
static size_t* test_2nd_key_getter_param_key_size;

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
    const char* INDEX_NAME = "test-index";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_index_init(&index, &datastore, INDEX_NAME,
            &test_2nd_key_getter));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_index_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;
    test_2nd_key_getter_called = false;

    /* call to vcdb_database_index_get should succeed. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the secondary key method should NOT have been called */
    EXPECT_FALSE(test_2nd_key_getter_called);

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
    const char* INDEX_NAME = "test-index";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_index_init(&index, &datastore, INDEX_NAME,
            &test_2nd_key_getter));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_index_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;
    test_2nd_key_getter_called = false;

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
    EXPECT_FALSE(test_2nd_key_getter_called);

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
    const char* INDEX_NAME = "test-index";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_index_init(&index, &datastore, INDEX_NAME,
            &test_2nd_key_getter));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_index_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;
    test_2nd_key_getter_called = false;

    /* call to vcdb_database_index_get should fail (bad parameter). */
    ASSERT_EQ(VCDB_ERROR_WOULD_TRUNCATE,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the secondary key method should NOT have been called */
    EXPECT_FALSE(test_2nd_key_getter_called);

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
    const char* INDEX_NAME = "test-index";

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_index_init(&index, &datastore, INDEX_NAME,
            &test_2nd_key_getter));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_create_from_builder(&database, &builder));

    /* preconditions */
    ASSERT_FALSE(test_index_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = -12;
    test_2nd_key_getter_called = false;

    /* call to vcdb_database_index_get should fail (bad parameter). */
    ASSERT_EQ(-12,
        vcdb_database_index_get(
            &database, &index,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the secondary key method should NOT have been called */
    EXPECT_FALSE(test_2nd_key_getter_called);

    /* the index_get method should have been called */
    EXPECT_TRUE(test_index_get_called);

    /* the serialization reader method should have been called. */
    EXPECT_TRUE(test_value_reader_called);

    /* clean up */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Initialize a dummy datastore for testing.
 *
 * \param datastore         The datastore to initialize.
 *
 * \returns a status code indicating success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - A non-zero status code on failure.
 */
static int test_datastore_init(vcdb_datastore_t* datastore)
{
    const char* NAME = "test_db";
    size_t SIZE = 128;

    //init should succeed
    return vcdb_datastore_init(
        datastore, NAME, SIZE, &test_key_getter, &test_value_reader,
        &test_value_writer);
}

static void test_key_getter(const void*, void*, size_t*)
{
    //NO-OP
}

static void test_2nd_key_getter(const void* value, void* key, size_t* key_size)
{
    test_2nd_key_getter_called = true;
    test_2nd_key_getter_param_value = value;
    test_2nd_key_getter_param_key = key;
    test_2nd_key_getter_param_key_size = key_size;
}

static int test_value_writer(const void*, void*, size_t*)
{
    //NO-OP
    return VCDB_STATUS_SUCCESS;
}

static int test_value_reader(const void* input, size_t size, void* value)
{
    test_value_reader_called = true;
    test_value_reader_param_input = input;
    test_value_reader_param_size = size;
    test_value_reader_param_value = value;

    return test_value_reader_retval;
}
