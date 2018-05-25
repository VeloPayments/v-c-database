/**
 * \file vcdb_database_create_from_builder.c
 *
 * \brief Implementation of the vcdb_database_create_from_builder() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vcdb/database.h>
#include <vpr/parameters.h>

#include "database_private.h"

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
    vcdb_builder_t* builder)
{
    /* TODO - add data structure invariant checks for database and builder. */
    MODEL_ASSERT(NULL != database);
    MODEL_ASSERT(NULL != builder);

    /* parameter check */
    if (
        NULL == database || NULL == builder)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* create the database using the engine-specific create method. */
    int retval = builder->engine->database_create(database, builder);

    /* if successful, set the opened flag and the builder pointer. */
    if (retval == VCDB_STATUS_SUCCESS)
    {
        builder->database_opened = true;
        database->hdr.dispose = &vcdb_database_dispose;
        database->builder = builder;
    }

    return retval;
}
