/**
 * \file database_private.h
 *
 * \brief Private details for the database interface.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_DATABASE_PRIVATE_HEADER_GUARD
#define VCDB_DATABASE_PRIVATE_HEADER_GUARD

#include <vcdb/database.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \brief Disposer for database.
 *
 * \param disposable        The disposable interface (database) to dispose.
 */
void vcdb_database_dispose(void* disposable);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif /*VCDB_DATABASE_PRIVATE_HEADER_GUARD*/
