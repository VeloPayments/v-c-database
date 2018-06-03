/**
 * \file test_index.h
 *
 * \brief Private header for a test index which can be used for testing the
 * builder interface.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef TEST_INDEX_PRIVATE_HEADER_GUARD
#define TEST_INDEX_PRIVATE_HEADER_GUARD

#include <stdbool.h>
#include <vcdb/builder.h>
#include <vcdb/database.h>
#include <vcdb/datastore.h>
#include <vcdb/transaction.h>
#include <vcdb/index.h>

/**
 * \brief Initialize a test index with default test values.
 *
 * \param index             The index to initialize.
 * \param datastore         The datastore that this index backs.
 *
 * \returns a status code indicating success or failure.
 *      - VCDB_STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
int test_index_init(vcdb_index_t* index, vcdb_datastore_t* datastore);

/**
 * \brief Reset the test index mock.
 */
void test_index_reset();

/**
 * \brief Set to true if test_secondary_key_getter() is called.
 */
extern bool test_secondary_key_getter_called;

/**
 * \brief Value parameter for test_secondary_key_getter().
 */
extern const void* test_secondary_key_getter_param_value;

/**
 * \brief Key parameter for test_secondary_key_getter().
 */
extern void* test_secondary_key_getter_param_key;

/**
 * \brief Key Size parameter for test_secondary_key_getter().
 */
extern size_t* test_secondary_key_getter_param_key_size;

#endif /*TEST_INDEX_PRIVATE_HEADER_GUARD*/
