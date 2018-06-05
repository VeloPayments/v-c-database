/**
 * \file test_datastore.h
 *
 * \brief Private header for a test datastore which can be used for testing the
 * builder interface.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef TEST_DATASTORE_PRIVATE_HEADER_GUARD
#define TEST_DATASTORE_PRIVATE_HEADER_GUARD

#include <stdbool.h>
#include <vcdb/builder.h>
#include <vcdb/database.h>
#include <vcdb/datastore.h>
#include <vcdb/transaction.h>
#include <vcdb/index.h>

/**
 * Data structure used for testing.
 */
typedef struct test_value
{
    char test_key[40];
    char test_value[100];
} test_value_t;

/**
 * Initialize a test datastore with default test values.
 *
 * \param datastore         The datastore to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - VCDB_STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
int test_datastore_init(vcdb_datastore_t* datastore);

/**
 * Reset the values for the test datastore.
 */
void test_datastore_reset();

/**
 * \brief Set to true if test_key_getter() is called.
 */
extern bool test_key_getter_called;

/**
 * \brief Value parameter of test_key_getter().
 */
extern const void* test_key_getter_param_value;

/**
 * \brief Key parameter of test_key_getter().
 */
extern void* test_key_getter_param_key;

/**
 * \brief Key size parameter of test_key_getter().
 */
extern size_t* test_key_getter_param_key_size;

/**
 * \brief Set to true if test_value_reader() is called.
 */
extern bool test_value_reader_called;

/**
 * \brief Return value for test_value_reader().
 */
extern int test_value_reader_retval;

/**
 * \brief Input parameter for test_value_reader().
 */
extern const void* test_value_reader_param_input;

/**
 * \brief Size parameter for test_value_reader().
 */
extern size_t test_value_reader_param_size;

/**
 * \brief Value parameter for test_value_reader().
 */
extern void* test_value_reader_param_value;

/**
 * \brief Set to true if test_value_writer() is called.
 */
extern bool test_value_writer_called;

/**
 * \brief Return value for test_value_writer().
 */
extern int test_value_writer_retval;

/**
 * \brief Value parameter for test_value_writer().
 */
extern const void* test_value_writer_param_value;

/**
 * \brief Output parameter for test_value_writer().
 */
extern void* test_value_writer_param_output;

/**
 * \brief Size parameter for test_value_writer().
 */
extern size_t* test_value_writer_param_size;

#endif /*TEST_DATASTORE_PRIVATE_HEADER_GUARD*/
