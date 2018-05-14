/**
 * \file vcdb_index_init.c
 *
 * \brief Implementation of the vcdb_index_init() function.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vcdb/datastore.h>
#include <vcdb/index.h>
#include <vpr/parameters.h>

/* forward decls */
static void vcdb_index_dispose(void* disposable);

/**
 * \brief Initialize a secondary index from a datastore, a name, and a secondary
 * key getter function.
 *
 * The initialized data structure is owned by the caller and either must be
 * reclaimed by calling dispose() or must have the ownership transferred to
 * another data structure.
 *
 * \param index     The index to initialize.
 * \param datastore The data store backing this index.
 * \param name      The unique name for this index, used to resolve this
 *                  instance.
 * \param getter    The secondary key getter method for this index.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_index_init(
    vcdb_index_t* index,
    vcdb_datastore_t* datastore,
    const char* name,
    vcdb_index_secondary_key_getter_method_t key_getter)
{
    MODEL_ASSERT(NULL != index);
    MODEL_ASSERT(NULL != datastore);
    MODEL_ASSERT(NULL != name);
    MODEL_ASSERT(NULL != key_getter);

    if (
        NULL == index ||
        NULL == datastore ||
        NULL == name ||
        NULL == key_getter)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    memset(index, 0, sizeof(vcdb_index_t));
    index->hdr.dispose = &vcdb_index_dispose;
    index->datastore = datastore;
    index->name = name;
    index->secondary_key_getter = key_getter;

    return VCDB_STATUS_SUCCESS;
}

/**
 * \brief Dispose of an index structure.
 *
 * \param disposable        The structure to dispose.
 */
static void vcdb_index_dispose(void* disposable)
{
    vcdb_index_t* index = (vcdb_index_t*)disposable;

    memset(index, 0, sizeof(vcdb_index_t));
}
