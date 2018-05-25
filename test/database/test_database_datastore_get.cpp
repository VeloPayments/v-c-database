/**
 * \file test_database_datastore_get.cpp
 *
 * \brief Test the vcdb_database_datastore_get() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/database.h>

#include "../test_database.h"

/* forward decls */
static int test_datastore_init(vcdb_datastore_t*);
static void test_key_getter(const void* value, void* key, size_t* key_size);
static int test_value_reader(const void* input, size_t size, void* value);
static int test_value_writer(const void* value, void* output, size_t* size);

/* method call accessors for test_value_reader */
bool test_value_reader_called;
const void* test_value_reader_param_input;
size_t test_value_reader_param_size;
void* test_value_reader_param_value;
int test_value_reader_retval;

/**
 * Test that the datastore_get method calls the appropriate engine methods.
 */
TEST(database_datastore_get, e2e)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    const char* key = "TESTKEY";
    size_t key_size = strlen(key);
    char value[1024];
    size_t value_size = sizeof(value);

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
    ASSERT_FALSE(test_datastore_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;

    /* call to vcdb_database_datastore_get should succeed. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_datastore_get(
            &database, &datastore,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the datastore_get method should have been called */
    EXPECT_TRUE(test_datastore_get_called);
    EXPECT_EQ(&database, test_datastore_get_param_database);
    EXPECT_EQ(&datastore, test_datastore_get_param_datastore);
    EXPECT_EQ(key, test_datastore_get_param_key);
    EXPECT_EQ(key_size, test_datastore_get_param_key_size);
    EXPECT_NE(nullptr, test_datastore_get_param_value);
    EXPECT_NE(nullptr, test_datastore_get_param_value_size);

    /* the serialization reader method should have been called. */
    EXPECT_TRUE(test_value_reader_called);
    EXPECT_EQ(test_datastore_get_param_value, test_value_reader_param_input);
    EXPECT_EQ(*test_datastore_get_param_value_size,
        test_value_reader_param_size);
    EXPECT_EQ(value, test_value_reader_param_value);

    /* clean up */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that the datastore_get method fails on invalid parameters.
 */
TEST(database_datastore_get, bad_params)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    const char* key = "TESTKEY";
    size_t key_size = strlen(key);
    char value[1024];
    size_t value_size = sizeof(value);

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
    ASSERT_FALSE(test_datastore_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;

    /* call to vcdb_database_datastore_get should fail (bad parameter). */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_get(
            NULL, &datastore,
            (void*)key, key_size,
            (void*)value, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_get(
            &database, NULL,
            (void*)key, key_size,
            (void*)value, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_get(
            &database, &datastore,
            NULL, key_size,
            (void*)value, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_get(
            &database, &datastore,
            (void*)key, 0U,
            (void*)value, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_get(
            &database, &datastore,
            (void*)key, key_size,
            NULL, &value_size));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_datastore_get(
            &database, &datastore,
            (void*)key, key_size,
            (void*)value, NULL));

    /* the datastore_get method should NOT have been called */
    EXPECT_FALSE(test_datastore_get_called);

    /* the serialization reader method should NOT have been called. */
    EXPECT_FALSE(test_value_reader_called);

    /* clean up */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that the datastore_get method fails when the value size is too small for
 * the value type.
 */
TEST(database_datastore_get, bad_value_size)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    const char* key = "TESTKEY";
    size_t key_size = strlen(key);
    char value[1024];
    size_t value_size = 10;

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
    ASSERT_FALSE(test_datastore_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;

    /* call to vcdb_database_datastore_get should fail (would truncate). */
    ASSERT_EQ(VCDB_ERROR_WOULD_TRUNCATE,
        vcdb_database_datastore_get(
            &database, &datastore,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the datastore_get method should NOT have been called */
    EXPECT_FALSE(test_datastore_get_called);

    /* the serialization reader method should NOT have been called. */
    EXPECT_FALSE(test_value_reader_called);

    /* clean up */
    dispose((disposable_t*)&database);
    dispose((disposable_t*)&builder);
}

/**
 * Test that the datastore_get method fails when the serialization method fails.
 */
TEST(database_datastore_get, bad_serialization)
{
    vcdb_builder_t builder;
    vcdb_database_t database;
    vcdb_datastore_t datastore;
    const char* key = "TESTKEY";
    size_t key_size = strlen(key);
    char value[1024];
    size_t value_size = sizeof(value);

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
    ASSERT_FALSE(test_datastore_get_called);
    test_value_reader_called = false;
    test_value_reader_retval = -15;

    /* call to vcdb_database_datastore_get should fail (serialization fail). */
    ASSERT_NE(VCDB_STATUS_SUCCESS,
        vcdb_database_datastore_get(
            &database, &datastore,
            (void*)key, key_size,
            (void*)value, &value_size));

    /* the datastore_get method should have been called */
    EXPECT_TRUE(test_datastore_get_called);

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
