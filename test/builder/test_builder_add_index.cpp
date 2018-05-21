/**
 * \file test_builder_add_index.cpp
 *
 * \brief Test the vcdb_builder_add_index() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/builder.h>

#include "test_database.h"

/* forward decls */
static int test_index_init(vcdb_index_t*);

/**
 * Test that we can add an index to the builder.
 */
TEST(builder_add_index, add_index)
{
    vcdb_index_t index;
    vcdb_builder_t builder;

    /* register the test database engine. */
    register_test_database();

    /* create the index and builder */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test"));

    /* The builder has no entries in its instance array. */
    ASSERT_EQ(0U, builder.instance_array_size);
    /* force the index correlation id to some weird value. */
    index.correlation_id = 17;

    /* We can add the index to the builder. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_index(&builder, &index));

    /* the builder has one entry in its instance array. */
    EXPECT_EQ(1U, builder.instance_array_size);
    /* the instance type for this entry is an index. */
    EXPECT_EQ(VCDB_BUILDER_INSTANCE_TYPE_INDEX,
        builder.instance_array[0].instance_type);
    /* the instance points to our index. */
    EXPECT_EQ(&index, builder.instance_array[0].instance.index);
    /* the corelation ID is set appropriately. */
    EXPECT_EQ(0, index.correlation_id);
    /* the handle is null. */
    EXPECT_EQ(nullptr, builder.instance_array[0].handle);

    dispose((disposable_t*)&builder);
}

/**
 * Test that parameter checks work.
 */
TEST(builder_add_index, bad_parameter)
{
    vcdb_index_t index;
    vcdb_builder_t builder;

    /* register the test database engine. */
    register_test_database();

    /* create the index and builder */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_index_init(&index));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test"));

    /* If the builder is null, this method fails. */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_builder_add_index(NULL, &index));
    /* If the index is null, this method fails. */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_builder_add_index(&builder, NULL));

    dispose((disposable_t*)&builder);
    dispose((disposable_t*)&index);
}

/**
 * Initialize a dummy index for testing.
 *
 * \param index         The index to initialize.
 *
 * \returns a status code indicating success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - A non-zero status code on failure.
 */
static int test_index_init(vcdb_index_t* index)
{
    const char* NAME = "test_idx";
    vcdb_datastore_t* dummy_datastore = (vcdb_datastore_t*)550;
    vcdb_index_secondary_key_getter_method_t GETTER =
        (vcdb_datastore_key_getter_method_t)440;

    //init should succeed
    return vcdb_index_init(index, dummy_datastore, NAME, GETTER);
}
