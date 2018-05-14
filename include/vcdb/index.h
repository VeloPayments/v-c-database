/**
 * \file index.h
 *
 * \brief The index interface provides a way to define a secondary index for a
 * key-value datastore.
 *
 * The index interface is an abstraction that encapsulates details relating to
 * the underlying secondary index.  This keeps the blockchain database agnostic.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_INDEX_HEADER_GUARD
#define VCDB_INDEX_HEADER_GUARD

#include <vcdb/datastore.h>
#include <vcdb/error_codes.h>
#include <vpr/disposable.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <stdlib.h>

/**
 * \brief Get the secondary key value given the data structure.
 *
 * \param value     The value being interrogated.
 * \param key       Pointer to be updated as the secondary key.  At least
 *                  VCDB_MAX_KEY_SIZE bytes in size.
 */
typedef void (*vcdb_index_secondary_key_getter_method_t)(
    const void* value, void* key, size_t* key_size);

/**
 * \brief This structure contains instance information for a given secondary
 * index.
 *
 * The secondary index instance information includes the datastore used for the
 * primary index, and a name for this secondary index.
 */
typedef struct vcdb_index
{
    /**
     * \brief This data structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief A reference to the primary datastore.
     */
    vcdb_datastore_t* datastore;

    /**
     * \brief An internal correlation ID that is used to link this secondary
     * index to data in the handle.
     */
    int correlation_id;

    /**
     * \brief The name of the secondary index.
     */
    const char* name;

    /**
     * \brief Get the secondary key value given the data structure.
     */
    vcdb_index_secondary_key_getter_method_t secondary_key_getter;

} vcdb_index_t;

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
    vcdb_index_secondary_key_getter_method_t key_getter);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_INDEX_HEADER_GUARD*/
