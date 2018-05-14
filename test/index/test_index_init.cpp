/**
 * \file test_index_init.cpp
 *
 * \brief Test the vcdb_index_init() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/index.h>

/**
 * Test that the init method works as expected.
 */
TEST(index_init_test, init)
{
    const char* NAME = "test_db";
    const char* INDEX_NAME = "test_index";
    size_t SIZE = 128;
    vcdb_datastore_key_getter_method_t GETTER =
        (vcdb_datastore_key_getter_method_t)110;
    vcdb_datastore_value_reader_method_t READER =
        (vcdb_datastore_value_reader_method_t)220;
    vcdb_datastore_value_writer_method_t WRITER =
        (vcdb_datastore_value_writer_method_t)330;
    vcdb_index_secondary_key_getter_method_t INDEX_GETTER =
        (vcdb_index_secondary_key_getter_method_t)440;
    vcdb_datastore_t store;
    vcdb_index_t index;

    //datastore init should succeed
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_datastore_init(&store, NAME, SIZE, GETTER, READER, WRITER));

    //index init should succeed
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_index_init(&index, &store, INDEX_NAME, INDEX_GETTER));

    //hdr.dispose should be set
    ASSERT_NE(nullptr, index.hdr.dispose);
    //datastore should be ours
    EXPECT_EQ(&store, index.datastore);
    //correlation ID should be 0
    EXPECT_EQ(0, index.correlation_id);
    //name should be ours
    EXPECT_EQ(INDEX_NAME, index.name);
    //secondary key getter should be ours
    EXPECT_EQ(INDEX_GETTER, index.secondary_key_getter);

    //clean up
    dispose((disposable_t*)&index);
    dispose((disposable_t*)&store);
}

/**
 * Test that calling the init method with an invalid parameter returns an error
 * code.
 */
TEST(index_init_test, init_invalid_parameter)
{
    const char* NAME = "test_db";
    const char* INDEX_NAME = "test_index";
    size_t SIZE = 128;
    vcdb_datastore_key_getter_method_t GETTER =
        (vcdb_datastore_key_getter_method_t)110;
    vcdb_datastore_value_reader_method_t READER =
        (vcdb_datastore_value_reader_method_t)220;
    vcdb_datastore_value_writer_method_t WRITER =
        (vcdb_datastore_value_writer_method_t)330;
    vcdb_index_secondary_key_getter_method_t INDEX_GETTER =
        (vcdb_index_secondary_key_getter_method_t)440;
    vcdb_datastore_t store;
    vcdb_index_t index;

    //datastore init should succeed
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_datastore_init(&store, NAME, SIZE, GETTER, READER, WRITER));

    //index init should succeed
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_index_init(nullptr, &store, INDEX_NAME, INDEX_GETTER));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_index_init(&index, nullptr, INDEX_NAME, INDEX_GETTER));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_index_init(&index, &store, nullptr, INDEX_GETTER));
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_index_init(&index, &store, INDEX_NAME, nullptr));

    //clean up
    dispose((disposable_t*)&store);
}
