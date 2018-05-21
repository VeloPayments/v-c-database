/**
 * \file vcdb_database_engine_lookup.c
 *
 * \brief Implementation of the vcdb_database_engine_lookup() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vcdb/engine.h>
#include <vpr/parameters.h>

#include "vcdb_database_engine.h"

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
vcdb_database_engine_t*
vcdb_database_engine_lookup(
    const char* engine)
{
    /* scan the registry for this engine */
    for (size_t i = 0; i < vcdb_database_engine_registry_size; ++i)
    {
        /* if the entry has been found... */
        if (!strcmp(engine, vcdb_database_engine_registry[i].name))
        {
            /* ...return it. */
            return vcdb_database_engine_registry[i].engine;
        }
    }

    /* the engine was not found in the registry. */
    return NULL;
}
