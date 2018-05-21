/**
 * \file test_builder_init.cpp
 *
 * \brief Test the vcdb_builder_init() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vcdb/builder.h>

#include "test_database.h"

/**
 * Test that the init method returns an error indicating that the given engine
 * was not found.
 */
TEST(builder_init, init_bad_engine)
{
    vcdb_builder_t builder;

    ASSERT_EQ(VCDB_ERROR_MISSING_DATABASE_ENGINE,
        vcdb_builder_init(&builder, "FOODB", "foodir"));
}

/**
 * Test that the init method initializes the builder with our test engine.
 */
TEST(builder_init, init)
{
    vcdb_builder_t builder;

    /* register the test database engine. */
    register_test_database();

    /* we should be able to build a test database. */
    ASSERT_EQ(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", "test-dir"));

    /* dispose our structure. */
    dispose((disposable_t*)&builder);
}

/**
 * Test that the init method fails on bad parameters.
 */
TEST(builder_init, init_bad_parameters)
{
    vcdb_builder_t builder;

    /* register the test database engine. */
    register_test_database();

    /* Init should fail on a bad builder parameter. */
    ASSERT_NE(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(NULL, "TESTDB", "test-dir"));

    /* Init should fail on a bad database engine parameter. */
    ASSERT_NE(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, NULL, "test-dir"));

    /* Init should fail on a bad connection string parameter. */
    ASSERT_NE(VCDB_STATUS_SUCCESS,
        vcdb_builder_init(&builder, "TESTDB", NULL));
}
