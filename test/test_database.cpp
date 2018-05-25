/**
 * \file test_database.cpp
 *
 * \brief Simple database interface mock for testing.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <vpr/parameters.h>

#include "test_database.h"

/* forward decls */
static void test_database_dispose(void* disposable);

/* internal data for registering a database instance. */
static bool test_database_registered = false;
static vcdb_database_engine_t test_database_engine = {
    &test_database_create,
    &test_database_open,
    &test_database_delete,
    &test_datastore_get,
    &test_index_get
};

/**
 * \brief Register the test database mock.
 */
void register_test_database()
{
    if (!test_database_registered)
    {
        vcdb_database_engine_register(&test_database_engine, "TESTDB");
        test_database_registered = true;
    }

    /* reset our mock */
    test_database_create_called = false;
    test_database_create_retval = VCDB_STATUS_SUCCESS;
    test_database_open_called = false;
    test_database_open_retval = VCDB_STATUS_SUCCESS;
    test_database_delete_called = false;
    test_database_delete_retval = VCDB_STATUS_SUCCESS;
    test_datastore_get_called = false;
    test_datastore_get_retval = VCDB_STATUS_SUCCESS;
    test_index_get_called = false;
    test_index_get_retval = VCDB_STATUS_SUCCESS;
}

/**
 * \brief Database engine method for creating a database.
 *
 * \param database      The database to create.
 * \param builder       The builder from which the database is created.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int test_database_create(
    struct vcdb_database* database,
    struct vcdb_builder* builder)
{
    test_database_create_called = true;
    test_database_create_param_database = database;
    test_database_create_param_builder = builder;

    if (NULL != database)
    {
        database->hdr.dispose = &test_database_dispose;
    }

    return test_database_create_retval;
}

/**
 * \brief Flag to indicate whether test_database_create() was called.
 */
bool test_database_create_called;

/**
 * \brief The return value for test_database_create().
 */
int test_database_create_retval;

/**
 * \brief The database parameter passed to test_database_create().
 */
vcdb_database_t* test_database_create_param_database;

/**
 * \brief The builder parameter passed to test_database_create().
 */
vcdb_builder_t* test_database_create_param_builder;

/**
 * \brief Database engine method for opening a database.
 *
 * \param database  The database instance to open.
 * \param builder   The builder to use to open this database.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int test_database_open(
    struct vcdb_database* database,
    struct vcdb_builder* builder)
{
    test_database_open_called = true;
    test_database_open_param_database = database;
    test_database_open_param_builder = builder;

    if (NULL != database)
    {
        database->hdr.dispose = &test_database_dispose;
    }

    return test_database_open_retval;
}

/**
 * \brief Flag to indicate whether test_database_open() was called.
 */
bool test_database_open_called;

/**
 * \brief The return value for test_database_open().
 */
int test_database_open_retval;

/**
 * \brief The database parameter passed to test_database_open().
 */
vcdb_database_t* test_database_open_param_database;

/**
 * \brief The builder parameter passed to test_database_open().
 */
vcdb_builder_t* test_database_open_param_builder;

/**
 * \brief Database engine method for deleting a database.
 *
 * \param builder   The builder to use to delete this database.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int test_database_delete(
    struct vcdb_builder* builder)
{
    test_database_delete_called = true;
    test_database_delete_param_builder = builder;

    return test_database_delete_retval;
}

/**
 * \brief Flag to indicate whether test_database_delete() was called.
 */
bool test_database_delete_called;

/**
 * \brief The return value for test_database_delete().
 */
int test_database_delete_retval;

/**
 * \brief The builder parameter passed to test_database_delete().
 */
vcdb_builder_t* test_database_delete_param_builder;

/**
 * \brief Database engine method for getting a value from a datastore.
 *
 * \param database      The database instance to use.
 * \param datastore     The datastore to get the value from.
 * \param key           The key to use for the query.
 * \param key_size      The size of the key.
 * \param value         The value to read.
 * \param value_size    The size pointer.  Must be set to the maximum size of
 *                      the value buffer.  On success, this pointer is updated
 *                      to the size of the data read.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - VCDB_ERROR_VALUE_NOT_FOUND if the value is not in the datastore.
 *          - VCDB_ERROR_WOULD_TRUNCATE if the value_size is too small.
 *          - a non-zero failure code on failure.
 */
int test_datastore_get(
    struct vcdb_database* database,
    struct vcdb_datastore* datastore,
    void* key,
    size_t key_size,
    void* value,
    size_t* value_size)
{
    test_datastore_get_called = true;
    test_datastore_get_param_database = database;
    test_datastore_get_param_datastore = datastore;
    test_datastore_get_param_key = key;
    test_datastore_get_param_key_size = key_size;
    test_datastore_get_param_value = value;
    test_datastore_get_param_value_size = value_size;

    return test_datastore_get_retval;
}

/**
 * \brief Flag to indicate whether test_datastore_get() was called.
 */
bool test_datastore_get_called;

/**
 * \brief The return value for test_datastore_get().
 */
int test_datastore_get_retval;

/**
 * \brief The database parameter passed to test_datastore_get().
 */
vcdb_database_t* test_datastore_get_param_database;

/**
 * \brief The datastore parameter passed to test_datastore_get().
 */
vcdb_datastore_t* test_datastore_get_param_datastore;

/**
 * \brief The key parameter passed to test_datastore_get().
 */
void* test_datastore_get_param_key;

/**
 * \brief The key_size parameter passed to test_datastore_get().
 */
size_t test_datastore_get_param_key_size;

/**
 * \brief The value parameter passed to test_datastore_get().
 */
void* test_datastore_get_param_value;

/**
 * \brief The value_size parameter passed to test_datastore_get().
 */
size_t* test_datastore_get_param_value_size;

/**
 * \brief Database engine method for getting a value from a secondary index.
 *
 * \param database      The database instance to use.
 * \param index         The secondary index to use when getting the value.
 * \param key           The key to use for the query.
 * \param key_size      The size of the key.
 * \param value         The value to read.
 * \param value_size    The size pointer.  Must be set to the maximum size of
 *                      the value buffer.  On success, this pointer is updated
 *                      to the size of the data read.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - VCDB_ERROR_VALUE_NOT_FOUND if the value is not in the
 *            index/datastore.
 *          - VCDB_ERROR_WOULD_TRUNCATE if the value_size is too small.
 *          - a non-zero failure code on failure.
 */
int test_index_get(
    struct vcdb_database* database,
    struct vcdb_index* index,
    void* key,
    size_t key_size,
    void* value,
    size_t* value_size)
{
    test_index_get_called = true;
    test_index_get_param_database = database;
    test_index_get_param_index = index;
    test_index_get_param_key = key;
    test_index_get_param_key_size = key_size;
    test_index_get_param_value = value;
    test_index_get_param_value_size = value_size;

    return test_index_get_retval;
}

/**
 * \brief Flag to indicate whether test_index_get() was called.
 */
bool test_index_get_called;

/**
 * \brief The return value for test_index_get().
 */
int test_index_get_retval;

/**
 * \brief The database parameter passed to test_index_get().
 */
vcdb_database_t* test_index_get_param_database;

/**
 * \brief The index parameter passed to test_index_get().
 */
vcdb_index_t* test_index_get_param_index;

/**
 * \brief The key parameter passed to test_index_get().
 */
void* test_index_get_param_key;

/**
 * \brief The key_size parameter passed to test_index_get().
 */
size_t test_index_get_param_key_size;

/**
 * \brief The value parameter passed to test_index_get().
 */
void* test_index_get_param_value;

/**
 * \brief The value_size parameter passed to test_index_get().
 */
size_t* test_index_get_param_value_size;

/**
 * \brief Dispose of the test database (do nothing).
 */
static void test_database_dispose(void* UNUSED(disposable))
{
}
