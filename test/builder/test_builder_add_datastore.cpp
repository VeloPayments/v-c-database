/**
 * \file test_builder_add_datastore.cpp
 *
 * \brief Test the vcdb_builder_add_datastore() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/builder.h>

#include "test_database.h"

/* forward decls */
static int test_datastore_init(vcdb_datastore_t*);

/**
 * Test that we can add a datastore to the builder.
 */
TEST(builder_add_datastore, add_datastore)
{
    vcdb_datastore_t datastore;
    vcdb_builder_t builder;

    /* register the test database engine. */
    register_test_database();

    /* create the datastore and builder */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test"));

    /* The builder has no entries in its instance array. */
    ASSERT_EQ(0U, builder.instance_array_size);

    /* We can add the datastore to the builder. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_add_datastore(&builder, &datastore));

    /* the builder has one entry in its instance array. */
    EXPECT_EQ(1U, builder.instance_array_size);
    /* the instance type for this entry is a datastore. */
    EXPECT_EQ(VCDB_BUILDER_INSTANCE_TYPE_DATASTORE,
        builder.instance_array[0].instance_type);
    /* the instance points to our datastore. */
    EXPECT_EQ(&datastore, builder.instance_array[0].instance.datastore);
    /* the handle is null. */
    EXPECT_EQ(nullptr, builder.instance_array[0].handle);

    dispose((disposable_t*)&builder);
}

/**
 * Test that parameter checks work.
 */
TEST(builder_add_datastore, bad_parameter)
{
    vcdb_datastore_t datastore;
    vcdb_builder_t builder;

    /* register the test database engine. */
    register_test_database();

    /* create the datastore and builder */
    ASSERT_EQ(VCDB_STATUS_SUCCESS, test_datastore_init(&datastore));
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test"));

    /* If the builder is null, this method fails. */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_builder_add_datastore(NULL, &datastore));
    /* If the datastore is null, this method fails. */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_builder_add_datastore(&builder, NULL));

    dispose((disposable_t*)&builder);
    dispose((disposable_t*)&datastore);
}

/**
 * Initialize a dummy datastore for testing.
 *
 * \param datastore         The datastore to initialize.
 *
 * \returns a status code indicating success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - A non-zero status code on failure.
 */
static int test_datastore_init(vcdb_datastore_t* datastore)
{
    const char* NAME = "test_db";
    size_t SIZE = 128;
    vcdb_datastore_key_getter_method_t GETTER =
        (vcdb_datastore_key_getter_method_t)110;
    vcdb_datastore_value_reader_method_t READER =
        (vcdb_datastore_value_reader_method_t)220;
    vcdb_datastore_value_writer_method_t WRITER =
        (vcdb_datastore_value_writer_method_t)330;

    //init should succeed
    return vcdb_datastore_init(datastore, NAME, SIZE, GETTER, READER, WRITER);
}
