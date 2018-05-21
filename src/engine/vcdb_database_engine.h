/**
 * \file vcdb_database_engine.h
 *
 * \brief Private header for the database engine.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_PRIVATE_DATABASE_ENGINE_HEADER_GUARD
#define VCDB_PRIVATE_DATABASE_ENGINE_HEADER_GUARD

#include <vcdb/engine.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <stdlib.h>

/**
 * \brief Internal data structure used for maintaining database engine entries.
 */
typedef struct vcdb_database_engine_entry
{
    char* name;
    vcdb_database_engine_t* engine;
} vcdb_database_engine_entry_t;

/**
 * \brief The internal database engine registry.
 *
 * This array is used by vcdb_database_engine_lookup() and
 * vcdb_database_engine_register().
 */
extern vcdb_database_engine_entry_t* vcdb_database_engine_registry;

/**
 * \brief The maximum size of the internal database engine registry.
 */
extern size_t vcdb_database_engine_registry_size_max;

/**
 * \brief The number of entries in the internal database engine registry.
 */
extern size_t vcdb_database_engine_registry_size;

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_PRIVATE_DATABASE_ENGINE_HEADER_GUARD*/
