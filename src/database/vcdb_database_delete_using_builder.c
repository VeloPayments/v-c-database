/**
 * \file vcdb_database_delete_using_builder.c
 *
 * \brief Implementation of the vcdb_database_delete_using_builder() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vcdb/database.h>
#include <vpr/parameters.h>

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
    vcdb_builder_t* builder)
{
    /* TODO - add data structure invariant checks for builder. */
    MODEL_ASSERT(NULL != builder);

    /* parameter check */
    if (
        NULL == builder)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* delete the database using the engine-specific delete method. */
    return builder->engine->database_delete(builder);
}
