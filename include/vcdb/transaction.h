/**
 * \file transaction.h
 *
 * \brief The transaction interface provides a way to change data in the
 * datastores and secondary indices for a given database.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_TRANSACTION_HEADER_GUARD
#define VCDB_TRANSACTION_HEADER_GUARD

#include <stdbool.h>
#include <vcdb/database.h>
#include <vpr/disposable.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

typedef struct vcdb_transaction
{
    disposable_t hdr;
    bool in_transaction;
    vcdb_database_t* database;
    void* transaction_engine_context;
} vcdb_transaction_t;

/**
 * \brief Begin a transaction using the given database.
 *
 * The database must stay in scope as long as the transaction is in scope.
 *
 * \param transaction   The transaction instance to create.
 * \param database      The database backing this transaction.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_transaction_begin(
    vcdb_transaction_t* transaction,
    vcdb_database_t* database);

/**
 * \brief Commit a transaction.
 *
 * After this action is perform, the transaction handle is disposed and is no
 * longer valid.  All changes made to the database using this interface will be
 * flushed to the database.
 *
 * \param transaction   The transaction instance to commit.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_transaction_commit(
    vcdb_transaction_t* transaction);

/**
 * \brief Roll back a transaction.
 *
 * After this action is perform, the transaction handle is disposed and is no
 * longer valid.  All changes made to the database using this interface will be
 * lost.
 *
 * \param transaction   The transaction instance to commit.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_transaction_rollback(
    vcdb_transaction_t* transaction);

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
    size_t* value_size);

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
    size_t* key_size);

/**
 * \brief Delete values matching the given key in the given secondary index.
 *
 * \param transaction   The transaction instance to use.
 * \param index         The secondary index to use when deleting.
 * \param key           The key to delete.
 * \param key_size      The size of the key to delete.
 *
 * \returns A status code signifying success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - a non-zero failure code on failure.
 */
int vcdb_database_index_delete(
    vcdb_transaction_t* transaction,
    vcdb_index_t* index,
    void* key,
    size_t* key_size);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_TRANSACTION_HEADER_GUARD*/
