/**
 * \file builder.h
 *
 * \brief The builder interface provides a way to build a database instance from
 * a sequence of datastores, indices, and options.
 *
 * The builder interface provides a way to build, create, destroy, and open
 * database instances.  The output of this interface is a vcdb_handle_t, which
 * is a handle to a particular database instances.  This can be used for
 * querying the database or for creating transactions to modify the database.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_BUILDER_HEADER_GUARD
#define VCDB_BUILDER_HEADER_GUARD

#include <vcdb/datastore.h>
#include <vcdb/engine.h>
#include <vcdb/error_codes.h>
#include <vcdb/index.h>
#include <vpr/disposable.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <stdbool.h>
#include <stdlib.h>

/**
 * \brief The number of instances to allocate by default.
 */
#define DEFAULT_INSTANCE_SIZE 20

/**
 * \brief This structure contains data about a datastore instance.
 *
 * The \ref instance value provides a type safe way to represent either
 * datastore or secondary index instance information.  The \ref instance_type
 * enumeration value provides a way to differentiate between these two handles.
 * Finally, the \ref handle value provides an opaque handle to the external
 * handle in the database engine for this particular object.
 */
typedef struct vcdb_builder_datastore_instance
{
    /**
     * \brief The instance pointer.
     */
    union
    {
        /**
         * \brief Pointer to a datastore instance.
         */
        vcdb_datastore_t* datastore;

        /**
         * \brief Pointer to a secondary index instance.
         */
        vcdb_index_t* index;

    } instance;

    /**
     * \brief The instance type.
     */
    enum
    {
        /**
         * \brief A datastore instance type.
         */
        VCDB_BUILDER_DATASTORE_TYPE_DATASTORE,

        /**
         * \brief A secondary index instance type.
         */
        VCDB_BUILDER_DATASTORE_TYPE_INDEX

    } instance_type;

    /**
     * \brief The handle to the external database object.
     */
    void* handle;

} vcdb_builder_datastore_instance_t;

/**
 * \brief This structure contains instance information for a given database
 * builder.
 *
 * The database builder instance must remain in scope while handles to a
 * database or transactions relating to a database are still in scope.
 */
typedef struct vcdb_builder
{
    /**
     * \brief This data structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The database engine for this builder instance.
     */
    vcdb_database_engine_t* engine;

    /**
     * \brief The connection string for this builder instance.
     */
    char* connection_string;

    /**
     * \brief An array of database instance information.
     */
    vcdb_builder_datastore_instance_t* instance_array;

    /**
     * \brief The maximum size of the instance array.
     */
    size_t instance_array_max;

    /**
     * \brief The current size of the instance array.
     */
    size_t instance_array_size;

    /**
     * \brief Set to true when the database has been opened.
     */
    bool database_opened;
} vcdb_builder_t;

/**
 * \brief Initialize a builder instance from an engine string and a connection
 * string.
 *
 * The engine string connects to a particular database engine that was
 * previously registered.  The connection string is used to resolve a particular
 * database instance and is implementation-specific.
 *
 * \param builder   The builder instance to initialize.
 * \param engine    The database engine to use.
 * \param connect   The implementation-dependent connection string for a
 *                  database instance.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_builder_init(
    vcdb_builder_t* builder,
    const char* engine,
    const char* connect);

/**
 * \brief Add a datastore to the builder.
 *
 * The builder takes ownership of this datastore and will dispose it when it is
 * disposed.
 *
 * \param builder   The builder instance to which this datastore is added.
 * \param datastore The datastore to add.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_builder_add_datastore(
    vcdb_builder_t* builder,
    vcdb_datastore_t* datastore);

/**
 * \brief Add a secondary index to the builder.
 *
 * The builder takes ownership of this index and will dispose it when it is
 * disposed.
 *
 * \param builder   The builder instance to which this datastore is added.
 * \param index     The index to add.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_builder_add_index(
    vcdb_builder_t* builder,
    vcdb_index_t* index);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_BUILDER_HEADER_GUARD*/
