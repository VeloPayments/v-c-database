/**
 * \file test_index.cpp
 *
 * \brief Mock implementation of index user callbacks.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include "test_datastore.h"

/* globals */
bool test_secondary_key_getter_called;
const void* test_secondary_key_getter_param_value;
void* test_secondary_key_getter_param_key;
size_t* test_secondary_key_getter_param_key_size;

/* forward decls */
static void test_secondary_key_getter(
    const void* value, void* key, size_t* key_size);

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
int test_index_init(vcdb_index_t* index, vcdb_datastore_t* datastore)
{
    const char* NAME = "test_idx";

    /* init should succeed */
    return vcdb_index_init(index, datastore, NAME, test_secondary_key_getter);
}

/**
 * \brief Reset the test index mock.
 */
void test_index_reset()
{
    test_secondary_key_getter_called = false;
}

/**
 * \brief Get the secondary key value given the data structure.
 *
 * \param value     The value being interrogated.
 * \param key       Pointer to be updated as the secondary key.  At least
 *                  VCDB_MAX_KEY_SIZE bytes in size.
 */
static void test_secondary_key_getter(
    const void* value, void* key, size_t* key_size)
{
    test_secondary_key_getter_called = true;
    test_secondary_key_getter_param_value = value;
    test_secondary_key_getter_param_key = key;
    test_secondary_key_getter_param_key_size = key_size;
}
