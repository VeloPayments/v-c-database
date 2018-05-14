/**
 * \file test_datastore_init.cpp
 *
 * \brief Test the datastore_init() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/datastore.h>

/**
 * Test that the init method works as expected.
 */
TEST(datastore_init_test, init)
{
    const char* NAME = "test_db";
    size_t SIZE = 128;
    vcdb_datastore_key_getter_method_t GETTER =
        (vcdb_datastore_key_getter_method_t)110;
    vcdb_datastore_value_reader_method_t READER =
        (vcdb_datastore_value_reader_method_t)220;
    vcdb_datastore_value_writer_method_t WRITER =
        (vcdb_datastore_value_writer_method_t)330;
    vcdb_datastore_t store;

    //init should succeed
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_datastore_init(&store, NAME, SIZE, GETTER, READER, WRITER));

    //hdr.dispose should be set
    ASSERT_NE(nullptr, store.hdr.dispose);
    //correlation ID should be 0
    EXPECT_EQ(0, store.correlation_id);
    //data_size should be SIZE
    EXPECT_EQ(SIZE, store.data_size);
    //key_getter should be GETTER
    EXPECT_EQ(GETTER, store.key_getter);
    //value_reader should be READER
    EXPECT_EQ(READER, store.value_reader);
    //value_writer should be WRITER
    EXPECT_EQ(WRITER, store.value_writer);
    //serial_data_size should be zero
    EXPECT_EQ(0U, store.serial_data_size);

    //clean up
    dispose((disposable_t*)&store);
}

/**
 * Test that calling the init method with an invalid parameter returns an error
 * code.
 */
TEST(datastore_init_test, init_invalid_parameter)
{
    const char* NAME = "test_db";
    size_t SIZE = 128;
    vcdb_datastore_key_getter_method_t GETTER =
        (vcdb_datastore_key_getter_method_t)110;
    vcdb_datastore_value_reader_method_t READER =
        (vcdb_datastore_value_reader_method_t)220;
    vcdb_datastore_value_writer_method_t WRITER =
        (vcdb_datastore_value_writer_method_t)330;
    vcdb_datastore_t store;

    //init should fail
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_datastore_init(nullptr, NAME, SIZE, GETTER, READER, WRITER));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_datastore_init(&store, nullptr, SIZE, GETTER, READER, WRITER));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_datastore_init(&store, NAME, 0, GETTER, READER, WRITER));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_datastore_init(&store, NAME, SIZE, nullptr, READER, WRITER));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_datastore_init(&store, NAME, SIZE, GETTER, nullptr, WRITER));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_datastore_init(&store, NAME, SIZE, GETTER, READER, nullptr));
}
