/**
 * \file vcdb_database_datastore_delete.c
 *
 * \brief Implementation of the vcdb_database_datastore_delete() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vcdb/transaction.h>
#include <vpr/parameters.h>

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
int vcdb_database_datastore_delete(
    vcdb_transaction_t* transaction,
    vcdb_datastore_t* datastore,
    void* key,
    size_t* key_size)
{
    MODEL_ASSERT(NULL != transaction);
    MODEL_ASSERT(NULL != datastore);
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(NULL != key_size);
    MODEL_ASSERT(0 != *key_size);

    /* parameter sanity check. */
    if (NULL == transaction || NULL == datastore || NULL == key || NULL == key_size || 0 == *key_size)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* make sure we are in a transaction. */
    if (!transaction->in_transaction)
    {
        return VCDB_ERROR_BAD_TRANSACTION;
    }

    /* delete the key using the engine method. */
    return transaction->database->builder->engine->datastore_delete(
        transaction, datastore, key, key_size);
}
