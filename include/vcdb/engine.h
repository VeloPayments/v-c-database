/**
 * \file engine.h
 *
 * \brief The engine interface provides a way for database implementations to be
 * provided as engines that can be used to interact with the Velo C Database
 * Interface Library.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_ENGINE_HEADER_GUARD
#define VCDB_ENGINE_HEADER_GUARD

#include <vcdb/error_codes.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <stdlib.h>

/* forward declarations for structures. */
struct vcdb_builder;
struct vcdb_database;
struct vcdb_datastore;
struct vcdb_index;

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
typedef int (*vcdb_database_engine_database_create_t)(
    struct vcdb_database* database,
    struct vcdb_builder* builder);

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
typedef int (*vcdb_database_engine_database_open_t)(
    struct vcdb_database* database,
    struct vcdb_builder* builder);

/**
 * \brief Database engine method for deleting a database.
 *
 * \param builder   The builder to use to delete this database.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
typedef int (*vcdb_database_engine_database_delete_t)(
    struct vcdb_builder* builder);

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
typedef int (*vcdb_database_engine_datastore_get_t)(
    struct vcdb_database* database,
    struct vcdb_datastore* datastore,
    void* key,
    size_t key_size,
    void* value,
    size_t* value_size);

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
typedef int (*vcdb_database_engine_index_get_t)(
    struct vcdb_database* database,
    struct vcdb_index* index,
    void* key,
    size_t key_size,
    void* value,
    size_t* value_size);

/**
 * \brief The database engine structure provides function pointers and context
 * information for a database engine implementation.
 */
typedef struct vcdb_database_engine
{
    /**
     * \brief Database engine method for creating a database.
     */
    vcdb_database_engine_database_create_t database_create;

    /**
     * \brief Database engine method for opening a database.
     */
    vcdb_database_engine_database_open_t database_open;

    /**
     * \brief Database engine method for deleting a database.
     */
    vcdb_database_engine_database_delete_t database_delete;

    /**
     * \brief Database engine method for getting a value from a datastore.
     */
    vcdb_database_engine_datastore_get_t datastore_get;

    /**
     * \brief Database engine method for getting a value from a secondary index.
     */
    vcdb_database_engine_index_get_t index_get;

} vcdb_database_engine_t;

/**
 * \brief This method looks up a database engine by name, returning a pointer to
 * the engine structure.
 *
 * This method is private to the database interface and the engine API, which is
 * used to register and look up database engine adapters.
 *
 * \param engine        The name of the database engine looked up by this
 *                      method.
 *
 * \returns the database engine associated with this name, or NULL if the engine
 * is not found.
 */
vcdb_database_engine_t* vcdb_database_engine_lookup(const char* engine);

/**
 * \brief This method registers a database engine with the given engine name.
 *
 * This method is private to the database interface and the engine API, which is
 * used to register and look up database engine adapters.
 *
 * \param eng           The engine to register.
 * \param engine        The name of the database engine under which this engine
 *                      is registered.
 */
void vcdb_database_engine_register(
    vcdb_database_engine_t* eng, const char* engine);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_ENGINE_HEADER_GUARD*/
