/**
 * \file vcdb_database_index_get.c
 *
 * \brief Implementation of the vcdb_database_index_get() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vcdb/database.h>
#include <vpr/parameters.h>

/* set a sane default for allocation. */
#ifndef VCDB_DATABASE_INDEX_GET_DEFAULT_DESERIALIZATION_BUFFER_SIZE
#define VCDB_DATABASE_INDEX_GET_DEFAULT_DESERIALIZATION_BUFFER_SIZE 1024
#endif

/**
 * \brief Get a value from the database via a secondary index corresponding to
 * a given key.
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
int vcdb_database_index_get(
    vcdb_database_t* database,
    vcdb_index_t* index,
    void* key,
    size_t key_size,
    void* value,
    size_t* value_size)
{
    /* TODO - add data structure invariant checks for database. */
    MODEL_ASSERT(NULL != database);
    MODEL_ASSERT(NULL != index);
    MODEL_ASSERT(NULL != index->datastore);
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(0 < key_size);
    MODEL_ASSERT(NULL != value);
    MODEL_ASSERT(NULL != value_size);
    MODEL_ASSERT(0 < *value_size);

    /* parameter check */
    if (
        NULL == database || NULL == index || NULL == index->datastore || NULL == key || 0 >= key_size || NULL == value || NULL == value_size || 0 >= *value_size)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* verify that the value size is correct for this type. */
    if (*value_size < index->datastore->data_size)
    {
        /* let the caller know how much data we need. */
        *value_size = index->datastore->data_size;

        return VCDB_ERROR_WOULD_TRUNCATE;
    }

    /* allocate temporary buffer for deserialization. */
    size_t buffer_size =
        VCDB_DATABASE_INDEX_GET_DEFAULT_DESERIALIZATION_BUFFER_SIZE;
    void* buffer = malloc(buffer_size);
    if (buffer == NULL)
    {
        return VCDB_ERROR_BAD_MEMORY_ALLOCATION;
    }

    /* read the data from the engine */
    int retval = database->builder->engine->index_get(
        database, index, key, key_size, buffer, &buffer_size);
    if (retval != VCDB_STATUS_SUCCESS && retval != VCDB_ERROR_WOULD_TRUNCATE)
    {
        goto cleanup_allocation;
    }
    /* was our buffer too small? */
    else if (retval == VCDB_ERROR_WOULD_TRUNCATE)
    {
        /* try to reallocate the buffer. */
        void* buf2 = realloc(buffer, buffer_size);
        if (NULL == buf2)
        {
            retval = VCDB_ERROR_BAD_MEMORY_ALLOCATION;
            goto cleanup_allocation;
        }
        buffer = buf2;

        /* retry the index_get with the larger size. */
        retval = database->builder->engine->index_get(
            database, index, key, key_size, buffer, &buffer_size);
        if (retval != VCDB_STATUS_SUCCESS)
        {
            goto cleanup_allocation;
        }
    }

    /* convert the serialized data back to the raw value. */
    retval = index->datastore->value_reader(buffer, buffer_size, value);

cleanup_allocation:
    free(buffer);

    return retval;
}
