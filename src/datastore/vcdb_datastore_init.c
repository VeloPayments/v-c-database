/**
 * \file vcdb_datastore_init.c
 *
 * \brief Implementation of the vcdb_datastore_init() function.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vcdb/datastore.h>
#include <vpr/parameters.h>

/* forward decls */
static void vcdb_datastore_dispose(void* disposable);

/**
 * \brief Initialize a datastore from a name, the data size, a key getter
 * function, a value reader function, and a value writer function.
 *
 * The initialized data structure is owned by the caller and either must be
 * reclaimed by calling dispose() or must have the ownership transferred to
 * another data structure.
 *
 * \param datastore The data store to initialize.
 * \param name      The unique name for this datastore, used to resolve this
 *                  instance.
 * \param size      The size of the value data to store in the data store.  Set
 *                  to 0 if this is a variable sized record.
 * \param getter    The key getter method for this record.
 * \param reader    The reader from serial data to record type for this record.
 * \param writer    The writer from record type to serial data for this record.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_datastore_init(
    vcdb_datastore_t* datastore,
    const char* name, size_t size,
    vcdb_datastore_key_getter_method_t key_getter,
    vcdb_datastore_value_reader_method_t value_reader,
    vcdb_datastore_value_writer_method_t value_writer)
{
    MODEL_ASSERT(NULL != datastore);
    MODEL_ASSERT(NULL != name);
    MODEL_ASSERT(0 != size);
    MODEL_ASSERT(NULL != key_getter);
    MODEL_ASSERT(NULL != value_reader);
    MODEL_ASSERT(NULL != value_writer);

    if (
        NULL == datastore ||
        NULL == name ||
        0 == size ||
        NULL == key_getter ||
        NULL == value_reader ||
        NULL == value_writer)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    memset(datastore, 0, sizeof(vcdb_datastore_t));
    datastore->hdr.dispose = &vcdb_datastore_dispose;
    datastore->name = name;
    datastore->data_size = size;
    datastore->key_getter = key_getter;
    datastore->value_reader = value_reader;
    datastore->value_writer = value_writer;

    return VCDB_STATUS_SUCCESS;
}

/**
 * \brief Dispose of a datastore structure.
 *
 * \param disposable        The structure to dispose.
 */
static void vcdb_datastore_dispose(void* disposable)
{
    vcdb_datastore_t* datastore = (vcdb_datastore_t*)disposable;

    memset(datastore, 0, sizeof(vcdb_datastore_t));
}
