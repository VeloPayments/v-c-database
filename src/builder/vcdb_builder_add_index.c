/**
 * \file vcdb_builder_add_index.c
 *
 * \brief Implementation of the vcdb_builder_add_index() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vcdb/builder.h>
#include <vpr/parameters.h>

#include "builder_private.h"

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
    vcdb_index_t* index)
{
    return vcdb_builder_add_generic(builder, index,
        VCDB_BUILDER_INSTANCE_TYPE_INDEX);
}
