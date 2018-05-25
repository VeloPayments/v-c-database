/**
 * \file test_database_delete_using_builder.cpp
 *
 * \brief Test the vcdb_database_delete_using_builder() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/database.h>

#include "../test_database.h"

/**
 * Test that the delete_using_builder method calls the appropriate engine
 * methods.
 */
TEST(database_delete_using_builder, init)
{
    vcdb_builder_t builder;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));

    /* preconditions */
    ASSERT_FALSE(test_database_delete_called);

    /* the opened flag is NOT set in the builder. */
    ASSERT_FALSE(builder.database_opened);

    /* we should be able to delete our database */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_database_delete_using_builder(
            &builder));

    /* the test_database_delete method was called. */
    ASSERT_TRUE(test_database_delete_called);
    EXPECT_EQ(&builder, test_database_delete_param_builder);

    /* the opened flag is NOT set in the builder. */
    EXPECT_FALSE(builder.database_opened);

    /* clean up. */
    dispose((disposable_t*)&builder);
}

/**
 * Test that passing bad parameters to delete_using_builder results in an error
 * return code.
 */
TEST(database_delete_using_builder, bad_param)
{
    vcdb_builder_t builder;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));

    /* test that bad parameters result in an error return code. */
    ASSERT_EQ(VCDB_ERROR_INVALID_PARAMETER,
        vcdb_database_delete_using_builder(
            NULL));

    /* clean up. */
    dispose((disposable_t*)&builder);
}

/**
 * Test that the return code from the engine delete method is returned by
 * delete_using_builder and if this is an error code, the database is not marked
 * as open.
 */
TEST(database_delete_using_builder, engine_error)
{
    vcdb_builder_t builder;
    int TEST_DATABASE_OPEN_ERROR_CODE = -1;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));

    /* preconditions */
    ASSERT_FALSE(test_database_delete_called);
    test_database_delete_retval = TEST_DATABASE_OPEN_ERROR_CODE;

    /* the opened flag is NOT set in the builder. */
    ASSERT_FALSE(builder.database_opened);

    /* the error code we return in the engine method is bubbled up. */
    ASSERT_EQ(TEST_DATABASE_OPEN_ERROR_CODE,
        vcdb_database_delete_using_builder(
            &builder));

    /* the test_database_delete method was called. */
    ASSERT_TRUE(test_database_delete_called);
    EXPECT_EQ(&builder, test_database_delete_param_builder);

    /* the opened flag is NOT set in the builder. */
    EXPECT_FALSE(builder.database_opened);

    /* clean up. */
    dispose((disposable_t*)&builder);
}
