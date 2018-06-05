/**
 * \file vcdb_database_datastore_put.c
 *
 * \brief Implementation of the vcdb_database_datastore_put() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vcdb/transaction.h>
#include <vpr/parameters.h>

/* set a sane default for allocation. */
#ifndef VCDB_DATABASE_DATASTORE_PUT_DEFAULT_SERIALIZATION_BUFFER_SIZE
#define VCDB_DATABASE_DATASTORE_PUT_DEFAULT_SERIALIZATION_BUFFER_SIZE 1024
#endif

/**
 * \brief Put a value into the datastore using the given transaction.
 *
 * If the value already exists, it will be updated.
 *
 * \param transaction   The transaction instance to use.
 * \param datastore     The datastore to put the value into.
 * \param value         The value to put.
 * \param value_size    The size of the value to put.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - a non-zero failure code on failure.
 */
int vcdb_database_datastore_put(
    vcdb_transaction_t* transaction,
    vcdb_datastore_t* datastore,
    void* value,
    size_t* value_size)
{
    int retval;

    MODEL_ASSERT(NULL != transaction);
    MODEL_ASSERT(NULL != datastore);
    MODEL_ASSERT(NULL != value);
    MODEL_ASSERT(NULL != value_size);
    MODEL_ASSERT(0 != *value_size);

    /* parameter sanity check. */
    if (NULL == transaction || NULL == datastore || NULL == value || NULL == value_size || 0 == *value_size)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* make sure we are in a transaction. */
    if (!transaction->in_transaction)
    {
        return VCDB_ERROR_BAD_TRANSACTION;
    }

    /* get the key from the value. */
    char key[VCDB_MAX_KEY_SIZE];
    size_t key_size = sizeof(key);
    datastore->key_getter(value, key, &key_size);

    /* allocate a sane default serialization buffer. */
    size_t allocation_size =
        VCDB_DATABASE_DATASTORE_PUT_DEFAULT_SERIALIZATION_BUFFER_SIZE;
    void* serialized_value = malloc(allocation_size);
    if (NULL == serialized_value)
    {
        return VCDB_ERROR_BAD_MEMORY_ALLOCATION;
    }

    /* serialize the value data. */
    retval =
        datastore->value_writer(value, serialized_value, &allocation_size);
    if (VCDB_ERROR_WOULD_TRUNCATE == retval)
    {
        /* reallocate a larger buffer. */
        void* newval = realloc(serialized_value, allocation_size);
        if (NULL == newval)
        {
            retval = VCDB_ERROR_BAD_MEMORY_ALLOCATION;
            goto cleanup_serial_buffer;
        }

        /* update the serial buffer. */
        serialized_value = newval;

        /* attempt serialization with the larger buffer. */
        retval =
            datastore->value_writer(value, serialized_value, &allocation_size);
        if (VCDB_STATUS_SUCCESS != retval)
        {
            goto cleanup_serial_buffer;
        }
    }

    /* if serialization fails, then clean up. */
    if (VCDB_STATUS_SUCCESS != retval)
    {
        goto cleanup_serial_buffer;
    }

    /* Put key and serialized value. */
    retval =
        transaction->database->builder->engine->datastore_put(
            transaction, datastore, serialized_value, &allocation_size);

cleanup_serial_buffer:
    free(serialized_value);

    return retval;
}
