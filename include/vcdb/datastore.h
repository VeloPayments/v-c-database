/**
 * \file datastore.h
 *
 * \brief The datastore interface provides a way to define a single key-value
 * datastore.
 *
 * The datastore interface is an abstraction that encapsulates details relating
 * to the underlying datastore.  This is done for two reasons.  First, it makes
 * the blockchain database agnostic, which makes it possible to work with many
 * different database implementations and platforms in client-side, mobile,
 * server-side, and embedded contexts.  Second, different database
 * implementations have different tradeoffs in terms of licensing and
 * administrative concerns.  While there are performance tradeoffs to consider
 * as well, we don't want to dictate expensive licensing to use the blockchain
 * library.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_DATASTORE_HEADER_GUARD
#define VCDB_DATASTORE_HEADER_GUARD

#include <vcdb/error_codes.h>
#include <vpr/disposable.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <stdlib.h>

#define VCDB_MAX_KEY_SIZE 1024

/**
 * \brief Get the key value given the data structure.
 *
 * \param value     The value being interrogated.
 * \param key       Pointer to be updated as the key.  At least
 *                  VCDB_MAX_KEY_SIZE bytes in size.
 */
typedef void (*vcdb_datastore_key_getter_method_t)(
    const void* value, void* key, size_t* key_size);

/**
 * \brief Read a value instance from serialized data.
 *
 * \param serial_input      The input data from which the value is read.
 * \param serial_input_size The size of the input data to be read.
 * \param value             The value to be read.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * A non-zero error code on failure.
 */
typedef int (*vcdb_datastore_value_reader_method_t)(
    const void* serial_input, size_t serial_input_size, void* value);

/**
 * \brief Write a value instance as serialized data.
 *
 * \param value             The value to serialize.
 * \param serial_output     The output buffer to write the serialized data to.
 *                          This is initially set to a fixed size that can be
 *                          configured for optimization.  However, if the size
 *                          is too small, a larger size can be requested by
 *                          updating serial_output_size to a larger size and
 *                          then returning with the error code
 *                          VCDB_ERROR_BUFFER_TOO_SMALL.  If possible, a larger
 *                          buffer will be allocated and this method will be
 *                          called again with this larger buffer.
 * \param serial_output_size This pointer is updated with a value representing
 *                          the size of the serialized data or the size for a
 *                          larger buffer to be requested as per the description
 *                          above.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * VCDB_ERROR_BUFFER_TOO_SMALL if the buffer is too small.  In this
 *            case, serial_output_size will be set to the size of buffer
 *            required for serialization.
 *          * A non-zero error code signifying error.
 */
typedef int (*vcdb_datastore_value_writer_method_t)(
    const void* value, void* serial_output, size_t* serial_output_size);

/**
 * \brief This structure contains instance information for a given datastore.
 *
 * The datastore instance information includes serialization methods which is
 * used to transform the internal structured data to a representation that can
 * be stored in the database.  Additionally, the size of the data structure,
 * and a method to get the key given this structure is required.
 */
typedef struct vcdb_datastore
{
    /**
     * \brief This data structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief An internal correlation ID that is used to link this datastore to
     * data in the handle.
     */
    int correlation_id;

    /**
     * \brief The name of the datastore.
     */
    const char* name;

    /**
     * \brief The size of the data structure used for values.
     */
    size_t data_size;

    /**
     * \brief Get the key value given the data structure.
     */
    vcdb_datastore_key_getter_method_t key_getter;

    /**
     * \brief Read a value instance from serialized data.
     */
    vcdb_datastore_value_reader_method_t value_reader;

    /**
     * \brief Write a value instance as serialized data.
     */
    vcdb_datastore_value_writer_method_t value_writer;

    /**
     * \brief Serial data size.
     */
    size_t serial_data_size;

} vcdb_datastore_t;

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
    vcdb_datastore_value_writer_method_t value_writer);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_DATASTORE_HEADER_GUARD*/
