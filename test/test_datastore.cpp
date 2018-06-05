/**
 * \file test_datastore.cpp
 *
 * \brief Mock implementation of datastore user callbacks.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cassert>
#include <cstring>
#include "test_datastore.h"

using namespace std;

/* globals */
bool test_key_getter_called;
const void* test_key_getter_param_value;
void* test_key_getter_param_key;
size_t* test_key_getter_param_key_size;
bool test_value_reader_called;
int test_value_reader_retval;
const void* test_value_reader_param_input;
size_t test_value_reader_param_size;
void* test_value_reader_param_value;
bool test_value_writer_called;
int test_value_writer_retval;
const void* test_value_writer_param_value;
void* test_value_writer_param_output;
size_t* test_value_writer_param_size;

/* forward decls */
static void test_key_getter(const void* value, void* key, size_t* key_size);
static int test_value_reader(const void* input, size_t size, void* value);
static int test_value_writer(const void* value, void* output, size_t* size);

/**
 * Initialize a test datastore with default test values.
 *
 * \param datastore         The datastore to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - VCDB_STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
int test_datastore_init(vcdb_datastore_t* datastore)
{
    const char* NAME = "test_db";
    size_t SIZE = sizeof(test_value);

    /* init should succeed */
    return vcdb_datastore_init(
        datastore, NAME, SIZE, &test_key_getter, &test_value_reader,
        &test_value_writer);
}

/**
 * Reset the values for the test datastore.
 */
void test_datastore_reset()
{
    test_key_getter_called = false;
    test_value_reader_called = false;
    test_value_reader_retval = VCDB_STATUS_SUCCESS;
    test_value_writer_called = false;
    test_value_writer_retval = VCDB_STATUS_SUCCESS;
}

/**
 * \brief User-supplied getter that returns the key for a given value.
 *
 * \param value         the value to interrogate.
 * \param key           the output buffer for the key.
 * \param key_size      pointer to the size of the key. On entry, set to the
 *                      maximum size of the key.  On exit, set to the size of
 *                      the key as written.
 */
static void test_key_getter(const void* value, void* key, size_t* key_size)
{
    test_key_getter_called = true;
    test_key_getter_param_value = value;
    test_key_getter_param_key = key;
    test_key_getter_param_key_size = key_size;

    test_value_t* v = (test_value_t*)value;
    assert(sizeof(v->test_key) < *key_size);
    memcpy(key, v->test_key, sizeof(v->test_key));
}

/**
 * \brief User-supplied value reader that creates a value from a serialized
 * input buffer.
 *
 * \param input         serialized data input.
 * \param size          size of the serialized data.
 * \param value         the value to create from this input.
 *
 * \returns A status code indicating success or failure.
 *      - VCDB_STATUS_SUCCESS on success.
 *      - A non-zero status value on failure.
 */
static int test_value_reader(const void* input, size_t size, void* value)
{
    test_value_reader_called = true;
    test_value_reader_param_input = input;
    test_value_reader_param_size = size;
    test_value_reader_param_value = value;

    return test_value_reader_retval;
}

/**
 * \brief User-supplied value writer that creates serialized output from a
 * value.
 *
 * \param value         the value to serialize.
 * \param output        the serialized data output value.
 * \param size          the size of the output buffer.  On entry, set to the
 *                      maximum size of the buffer. On exit, set to the size of
 *                      the output data on success, or the size required to
 *                      write to this buffer if the buffer is too small.
 *
 * \returns A status code indicating success or failure.
 *      - VCDB_STATUS_SUCCESS on success.
 *      - VCDB_ERROR_BUFFER_TOO_SMALL if the buffer is too small.  In this case,
 *         a size will be set to the size of a buffer required for
 *         serialization.
 *      - A non-zero status value on failure.
 */
static int test_value_writer(const void* value, void* output, size_t* size)
{
    test_value_writer_called = true;
    test_value_writer_param_value = value;
    test_value_writer_param_output = output;
    test_value_writer_param_size = size;

    return test_value_writer_retval;
}
