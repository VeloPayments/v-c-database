/**
 * \file vcdb_database_dispose.c
 *
 * \brief Implementation of the vcdb_database_dispose() method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <stdbool.h>
#include <string.h>

#include "database_private.h"

/**
 * \brief Disposer for database.
 *
 * \param disposable        The disposable interface (database) to dispose.
 */
void vcdb_database_dispose(void* disposable)
{
    vcdb_database_t* database = (vcdb_database_t*)disposable;

    /* close the database. */
    database->builder->engine->database_close(database);

    /* the database is no longer opened. */
    database->builder->database_opened = false;

    /* clear the database data structure. */
    memset(database, 0, sizeof(vcdb_database_t));
}
