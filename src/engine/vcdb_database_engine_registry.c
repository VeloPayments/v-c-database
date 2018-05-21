/**
 * \file vcdb_database_engine_registry.c
 *
 * \brief Global data for the database engine registry.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include "vcdb_database_engine.h"

vcdb_database_engine_entry_t* vcdb_database_engine_registry = NULL;
size_t vcdb_database_engine_registry_size_max = 0U;
size_t vcdb_database_engine_registry_size = 0U;
