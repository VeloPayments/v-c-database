/**
 * \file database.h
 *
 * \brief The database interface provides a way to read from datastores and
 * secondary indices.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_DATABASE_HEADER_GUARD
#define VCDB_DATABASE_HEADER_GUARD

#include <vcdb/builder.h>
#include <vpr/disposable.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \brief The database interface is used to perform operations on the database.
 *
 * With this interface, it is possible to create and open databases described by
 * a builder and get a read-only view to these databases.
 */
typedef struct vcdb_database
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief Weak reference to the builder, used for engine-specific
     * operations.
     */
    vcdb_builder_t* builder;
} vcdb_database_t;

/**
 * \brief Create a database from the given builder.
 *
 * The builder must stay in scope as long as the handle is in scope.  The handle
 * is owned by the caller and must be disposed of by calling dispose() on it.
 *
 * \param database  The database instance to create.
 * \param builder   The builder to use to create this database.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_database_create_from_builder(
    vcdb_database_t* database,
    vcdb_builder_t* builder);

/**
 * \brief Open a database from the given builder.
 *
 * The builder must stay in scope as long as the handle is in scope.  The handle
 * is owned by the caller and must be disposed of by calling dispose() on it.
 *
 * \param database  The database instance to open.
 * \param builder   The builder to use to open this database.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_database_open_from_builder(
    vcdb_database_t* database,
    vcdb_builder_t* builder);

/**
 * \brief Delete a database using the given builder.
 *
 * \param builder   The builder to use to delete this database.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_database_delete_using_builder(
    vcdb_builder_t* builder);

/**
 * \brief Get a value from the database corresponding to a given key.
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
int vcdb_database_datastore_get(
    vcdb_database_t* database,
    vcdb_datastore_t* datastore,
    void* key,
    size_t key_size,
    void* value,
    size_t* value_size);

/**
 * \brief Get a value from the database via a secondary index corresponding to
 * a given key.
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
int vcdb_database_index_get(
    vcdb_database_t* database,
    vcdb_index_t* index,
    void* key,
    size_t key_size,
    void* value,
    size_t* value_size);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_DATABASE_HEADER_GUARD*/
