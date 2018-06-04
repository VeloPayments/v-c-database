/**
 * \file test_database.h
 *
 * \brief Private header for a test database which can be used for testing the
 * builder interface.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef TEST_DATABASE_PRIVATE_HEADER_GUARD
#define TEST_DATABASE_PRIVATE_HEADER_GUARD

#include <stdbool.h>
#include <vcdb/builder.h>
#include <vcdb/database.h>
#include <vcdb/datastore.h>
#include <vcdb/transaction.h>
#include <vcdb/index.h>

extern int test_database_dummy;

/**
 * \brief Register the test database mock.
 */
void register_test_database();

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
    struct vcdb_builder* builder);

/**
 * \brief Flag to indicate whether test_database_create() was called.
 */
extern bool test_database_create_called;

/**
 * \brief The return value for test_database_create().
 */
extern int test_database_create_retval;

/**
 * \brief The database parameter passed to test_database_create().
 */
extern vcdb_database_t* test_database_create_param_database;

/**
 * \brief The builder parameter passed to test_database_create().
 */
extern vcdb_builder_t* test_database_create_param_builder;

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
    struct vcdb_builder* builder);

/**
 * \brief Flag to indicate whether test_database_open() was called.
 */
extern bool test_database_open_called;

/**
 * \brief The return value for test_database_open().
 */
extern int test_database_open_retval;

/**
 * \brief The database parameter passed to test_database_open().
 */
extern vcdb_database_t* test_database_open_param_database;

/**
 * \brief The builder parameter passed to test_database_open().
 */
extern vcdb_builder_t* test_database_open_param_builder;

/**
 * \brief Database engine method for closing a database.
 *
 * \param database  The database instance to close.
 */
void test_database_close(
    struct vcdb_database* database);

/**
 * \brief Flag to indicate whether test_database_close() was called.
 */
extern bool test_database_close_called;

/**
 * \brief The database parameter passed to test_database_close().
 */
extern vcdb_database_t* test_database_close_param_database;

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
    struct vcdb_builder* builder);

/**
 * \brief Flag to indicate whether test_database_delete() was called.
 */
extern bool test_database_delete_called;

/**
 * \brief The return value for test_database_delete().
 */
extern int test_database_delete_retval;

/**
 * \brief The builder parameter passed to test_database_delete().
 */
extern vcdb_builder_t* test_database_delete_param_builder;

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
    size_t* value_size);

/**
 * \brief Flag to indicate whether test_datastore_get() was called.
 */
extern bool test_datastore_get_called;

/**
 * \brief The return value for test_datastore_get().
 */
extern int test_datastore_get_retval;

/**
 * \brief The database parameter passed to test_datastore_get().
 */
extern vcdb_database_t* test_datastore_get_param_database;

/**
 * \brief The datastore parameter passed to test_datastore_get().
 */
extern vcdb_datastore_t* test_datastore_get_param_datastore;

/**
 * \brief The key parameter passed to test_datastore_get().
 */
extern void* test_datastore_get_param_key;

/**
 * \brief The key_size parameter passed to test_datastore_get().
 */
extern size_t test_datastore_get_param_key_size;

/**
 * \brief The value parameter passed to test_datastore_get().
 */
extern void* test_datastore_get_param_value;

/**
 * \brief The value_size parameter passed to test_datastore_get().
 */
extern size_t* test_datastore_get_param_value_size;

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
    size_t* value_size);

/**
 * \brief Flag to indicate whether test_index_get() was called.
 */
extern bool test_index_get_called;

/**
 * \brief The return value for test_index_get().
 */
extern int test_index_get_retval;

/**
 * \brief The database parameter passed to test_index_get().
 */
extern vcdb_database_t* test_index_get_param_database;

/**
 * \brief The index parameter passed to test_index_get().
 */
extern vcdb_index_t* test_index_get_param_index;

/**
 * \brief The key parameter passed to test_index_get().
 */
extern void* test_index_get_param_key;

/**
 * \brief The key_size parameter passed to test_index_get().
 */
extern size_t test_index_get_param_key_size;

/**
 * \brief The value parameter passed to test_index_get().
 */
extern void* test_index_get_param_value;

/**
 * \brief The value_size parameter passed to test_index_get().
 */
extern size_t* test_index_get_param_value_size;

/**
 * \brief Begin a transaction in the given database.
 *
 * \param transaction   The transaction instance to create.
 * \param database      The database to which this transaction belongs.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - a non-zero failure code on failure.
 */
int test_transaction_begin(
    struct vcdb_transaction* transaction,
    struct vcdb_database* database);

/**
 * \brief Flag to indicate whether test_transaction_begin() was called.
 */
extern bool test_transaction_begin_called;

/**
 * \brief The return value for test_transaction_begin().
 */
extern int test_transaction_begin_retval;

/**
 * \brief The transaction parameter passed to test_transaction_begin().
 */
extern vcdb_transaction_t* test_transaction_begin_param_transaction;

/**
 * \brief The database parameter passed to test_transaction_begin().
 */
extern vcdb_database_t* test_transaction_begin_param_database;

/**
 * \brief Commit a transaction.
 *
 * \param transaction   The transaction instance to commit.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - a non-zero failure code on failure.
 */
int test_transaction_commit(
    struct vcdb_transaction* transaction);

/**
 * \brief Flag to indicate whether test_transaction_commit() was called.
 */
extern bool test_transaction_commit_called;

/**
 * \brief The return value for test_transaction_commit().
 */
extern int test_transaction_commit_retval;

/**
 * \brief The transaction parameter passed to test_transaction_commit().
 */
extern vcdb_transaction_t* test_transaction_commit_param_transaction;

/**
 * \brief Roll back a transaction.
 *
 * \param transaction   The transaction instance to roll back.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - a non-zero failure code on failure.
 */
int test_transaction_rollback(
    struct vcdb_transaction* transaction);

/**
 * \brief Flag to indicate whether test_transaction_rollback() was called.
 */
extern bool test_transaction_rollback_called;

/**
 * \brief The return value for test_transaction_rollback().
 */
extern int test_transaction_rollback_retval;

/**
 * \brief The transaction parameter passed to test_transaction_rollback().
 */
extern vcdb_transaction_t* test_transaction_rollback_param_transaction;

/**
 * \brief Put a value into the datastore using the given transaction.
 *
 * If the value already exists, it will be updated.
 *
 * \param transaction   The transaction instance to use.
 * \param datastore     The datastore to put the value into.
 * \param value         The value to put.
 * \param value_size    The size of the value to put.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - a non-zero failure code on failure.
 */
int test_datastore_put(
    struct vcdb_transaction* transaction,
    struct vcdb_datastore* datastore,
    void* value,
    size_t* value_size);

/**
 * \brief Flag to indicate whether test_datastore_put() was called.
 */
extern bool test_datastore_put_called;

/**
 * \brief The return value for test_datastore_put().
 */
extern int test_datastore_put_retval;

/**
 * \brief The transaction parameter passed to test_datastore_put().
 */
extern vcdb_transaction_t* test_datastore_put_param_transaction;

/**
 * \brief The datastore parameter passed to test_datastore_put().
 */
extern vcdb_datastore_t* test_datastore_put_param_datastore;

/**
 * \brief The value parameter passed to test_datastore_put().
 */
extern void* test_datastore_put_param_value;

/**
 * \brief The value_size parameter passed to test_datastore_put().
 */
extern size_t* test_datastore_put_param_value_size;

/**
 * \brief Delete values matching the given key in the given datastore.
 *
 * \param transaction   The transaction instance to use.
 * \param datastore     The datastore to use when deleting.
 * \param key           The key to delete.
 * \param key_size      The size of the key to delete.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - a non-zero failure code on failure.
 */
int test_datastore_delete(
    struct vcdb_transaction* transaction,
    struct vcdb_datastore* datastore,
    void* key,
    size_t* key_size);

/**
 * \brief Flag to indicate whether test_datastore_delete() was called.
 */
extern bool test_datastore_delete_called;

/**
 * \brief The return value for test_datastore_delete().
 */
extern int test_datastore_delete_retval;

/**
 * \brief The transaction parameter passed to test_datastore_delete().
 */
extern vcdb_transaction_t* test_datastore_delete_param_transaction;

/**
 * \brief The datastore parameter passed to test_datastore_delete().
 */
extern vcdb_datastore_t* test_datastore_delete_param_datastore;

/**
 * \brief The key parameter passed to test_datastore_delete().
 */
extern void* test_datastore_delete_param_key;

/**
 * \brief The key_size parameter passed to test_datastore_delete().
 */
extern size_t* test_datastore_delete_param_key_size;

/**
 * \brief Delete values matching the given key in the given secondary index.
 *
 * \param transaction   The transaction instance to use.
 * \param index         The secondary index to use when deleting.
 * \param key           The key to delete.
 * \param key_size      The size of the key to delete.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - a non-zero failure code on failure.
 */
int test_index_delete(
    struct vcdb_transaction* transaction,
    struct vcdb_index* index,
    void* key,
    size_t* key_size);

/**
 * \brief Flag to indicate whether test_index_delete() was called.
 */
extern bool test_index_delete_called;

/**
 * \brief The return value for test_index_delete().
 */
extern int test_index_delete_retval;

/**
 * \brief The transaction parameter passed to test_index_delete().
 */
extern vcdb_transaction_t* test_index_delete_param_transaction;

/**
 * \brief The index parameter passed to test_index_delete().
 */
extern vcdb_index_t* test_index_delete_param_index;

/**
 * \brief The key parameter passed to test_index_delete().
 */
extern void* test_index_delete_param_key;

/**
 * \brief The key_size parameter passed to test_index_delete().
 */
extern size_t* test_index_delete_param_key_size;

#endif /*TEST_DATABASE_PRIVATE_HEADER_GUARD*/
