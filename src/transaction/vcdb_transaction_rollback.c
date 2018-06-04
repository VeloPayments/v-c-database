/**
 * \file vcdb_transaction_rollback.c
 *
 * \brief Implementation of the vcdb_transaction_rollback() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vcdb/transaction.h>
#include <vpr/parameters.h>

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
    vcdb_transaction_t* transaction)
{
    MODEL_ASSERT(NULL != transaction);

    /* parameter sanity check. */
    if (NULL == transaction)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* call the engine-specific transaction rollback procedure. */
    int retval = transaction->database->builder->engine->transaction_rollback(
        transaction);
    if (VCDB_STATUS_SUCCESS == retval)
    {
        transaction->in_transaction = false;
    }

    return retval;
}
