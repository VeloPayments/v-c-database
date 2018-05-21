/**
 * \file vcdb_builder_add_datastore.c
 *
 * \brief Implementation of the vcdb_builder_add_datastore() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vcdb/builder.h>
#include <vpr/parameters.h>

#include "builder_private.h"

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
    vcdb_datastore_t* datastore)
{
    return vcdb_builder_add_generic(builder, datastore,
        VCDB_BUILDER_INSTANCE_TYPE_DATASTORE);
}
