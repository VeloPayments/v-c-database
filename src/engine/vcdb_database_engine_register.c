/**
 * \file vcdb_database_engine_register.c
 *
 * \brief Implementation of the vcdb_database_engine_register() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vcdb/engine.h>
#include <vpr/parameters.h>

#include "vcdb_database_engine.h"

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
    vcdb_database_engine_t* eng, const char* engine)
{
    /* allocate the registry if it does not already exist. */
    if (vcdb_database_engine_registry_size_max == 0)
    {
        vcdb_database_engine_registry_size_max = 5;
        vcdb_database_engine_registry_size = 0;
        vcdb_database_engine_registry = (vcdb_database_engine_entry_t*)
            malloc(vcdb_database_engine_registry_size_max * sizeof(vcdb_database_engine_entry_t));
    }

    /* if the registry is NULL at this point, allocation failed. Clean up. */
    if (NULL == vcdb_database_engine_registry)
    {
        vcdb_database_engine_registry_size_max = 0;
        vcdb_database_engine_registry_size = 0;

        return;
    }

    /* if we can't register entries, increase the registry size. */
    if (vcdb_database_engine_registry_size == vcdb_database_engine_registry_size_max)
    {
        /* grow the array.  Return on failure. */
        void* newdata = realloc(vcdb_database_engine_registry,
            vcdb_database_engine_registry_size_max + 5);
        if (NULL == newdata)
        {
            return;
        }

        /* on success, switch out the array and increase the max size */
        vcdb_database_engine_registry = (vcdb_database_engine_entry_t*)newdata;
        vcdb_database_engine_registry_size_max += 5;
    }

    /* Set the new entry. */
    size_t offset = vcdb_database_engine_registry_size++;
    vcdb_database_engine_registry[offset].name = strdup(engine);
    vcdb_database_engine_registry[offset].engine = eng;

    /* edge case - strdup() fails.  Clean up by decrementing size. */
    if (NULL == vcdb_database_engine_registry[offset].name)
    {
        --vcdb_database_engine_registry_size;
    }
}
