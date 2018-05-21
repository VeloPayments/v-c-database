/**
 * \file engine.h
 *
 * \brief The engine interface provides a way for database implementations to be
 * provided as engines that can be used to interact with the Velo C Database
 * Interface Library.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_ENGINE_HEADER_GUARD
#define VCDB_ENGINE_HEADER_GUARD

#include <vcdb/error_codes.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <stdlib.h>

/**
 * \brief The database engine structure provides function pointers and context
 * information for a database engine implementation.
 */
typedef struct vcdb_database_engine
{
} vcdb_database_engine_t;

/**
 * \brief This method looks up a database engine by name, returning a pointer to
 * the engine structure.
 *
 * This method is private to the database interface and the engine API, which is
 * used to register and look up database engine adapters.
 *
 * \param engine        The name of the database engine looked up by this
 *                      method.
 *
 * \returns the database engine associated with this name, or NULL if the engine
 * is not found.
 */
vcdb_database_engine_t* vcdb_database_engine_lookup(const char* engine);

/**
 * \brief This method registers a database engine with the given engine name.
 *
 * This method is private to the database interface and the engine API, which is
 * used to register and look up database engine adapters.
 *
 * \param eng           The engine to register.
 * \param engine        The name of the database engine under which this engine
 *                      is registered.
 */
void vcdb_database_engine_register(
    vcdb_database_engine_t* eng, const char* engine);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_ENGINE_HEADER_GUARD*/
