/**
 * \file vcdb_builder_add_generic.c
 *
 * \brief Implementation of the vcdb_builder_add_generic() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vcdb/builder.h>
#include <vpr/parameters.h>

#include "builder_private.h"

/**
 * \brief Add a generalized datastore to the builder.
 *
 * The builder takes ownership of this datastore and will dispose it when it is
 * disposed.
 *
 * \param builder   The builder instance to which this datastore is added.
 * \param datastore The datastore to add.
 * \param type      The type of datastore to add.
 *
 * \returns A status code indicating success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - A non-zero failure code on failure.
 */
int vcdb_builder_add_generic(
    vcdb_builder_t* builder, void* datastore, int type)
{
    MODEL_ASSERT(builder != NULL);
    MODEL_ASSERT(datastore != NULL);
    MODEL_ASSERT(type == VCDB_BUILDER_INSTANCE_TYPE_DATASTORE || type == VCDB_BUILDER_INSTANCE_TYPE_INDEX);

    /* parameter sanity check. */
    if (builder == NULL || datastore == NULL || (type != VCDB_BUILDER_INSTANCE_TYPE_DATASTORE && type != VCDB_BUILDER_INSTANCE_TYPE_INDEX))
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* should we increase the instance size? */
    if (builder->instance_array_size == builder->instance_array_max)
    {
        /* grow the array.  Return on failure. */
        void* newdata = realloc(builder->instance_array,
            builder->instance_array_max + DEFAULT_INSTANCE_SIZE);
        if (NULL == newdata)
        {
            return VCDB_ERROR_BAD_MEMORY_ALLOCATION;
        }

        /* on success, switch out the array and increase the max size. */
        builder->instance_array = (vcdb_builder_datastore_instance_t*)newdata;
        builder->instance_array_max += DEFAULT_INSTANCE_SIZE;
    }

    /* set the new entry. */
    builder->instance_array[builder->instance_array_size].instance.datastore =
        (vcdb_datastore_t*)datastore;
    builder->instance_array[builder->instance_array_size].instance_type = type;
    builder->instance_array[builder->instance_array_size].handle = NULL;

    /* we now have one more entry. */
    ++builder->instance_array_size;

    return VCDB_STATUS_SUCCESS;
}
