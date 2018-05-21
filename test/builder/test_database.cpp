/**
 * \file test_database.cpp
 *
 * \brief Simple database interface mock for testing.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include "test_database.h"

/* internal data for registering a database instance. */
static bool test_database_registered = false;
static vcdb_database_engine_t test_database_engine = {};

/**
 * \brief Register the test database mock.
 */
void register_test_database()
{
    if (!test_database_registered)
    {
        vcdb_database_engine_register(&test_database_engine, "TESTDB");
        test_database_registered = true;
    }
}
