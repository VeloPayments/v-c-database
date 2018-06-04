/**
 * \file vcdb_transaction_begin.c
 *
 * \brief Implementation of the vcdb_transaction_begin() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vcdb/transaction.h>
#include <vpr/parameters.h>

static void vcdb_transaction_dispose(void* disposable);

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
    vcdb_database_t* database)
{
    MODEL_ASSERT(NULL != transaction);
    MODEL_ASSERT(NULL != database);

    /* parameter sanity check. */
    if (NULL == transaction || NULL == database)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* transaction is disposable. */
    transaction->hdr.dispose = &vcdb_transaction_dispose;
    transaction->database = database;

    /* engine-specific setup */
    int retval =
        database->builder->engine->transaction_begin(transaction, database);
    if (retval == VCDB_STATUS_SUCCESS)
    {
        transaction->in_transaction = true;
    }
    else
    {
        transaction->in_transaction = false;
    }

    return retval;
}

/**
 * \brief Dispose of a transaction.
 *
 * \param disposable        The transaction to dispose.
 */
static void vcdb_transaction_dispose(void* disposable)
{
    vcdb_transaction_t* transaction = (vcdb_transaction_t*)disposable;

    /* if the transaction is active, roll back. */
    if (transaction->in_transaction)
    {
        vcdb_transaction_rollback(transaction);
        transaction->in_transaction = false;
    }
}
