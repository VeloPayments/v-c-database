/**
 * \file builder_private.h
 *
 * \brief Private details about the builder.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_BUILDER_PRIVATE_HEADER_GUARD
#define VCDB_BUILDER_PRIVATE_HEADER_GUARD

#include <vcdb/builder.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \brief Add a generalized datastore to the builder.
 *
 * The builder takes ownership of this datastore and will dispose it when it is
 * disposed.
 *
 * \param builder   The builder instance to which this datastore is added.
 * \param datastore The datastore to add.
 * \param type      The type of datastore to add.
 *
 * \returns A status code indicating success or failure.
 *          - VCDB_STATUS_SUCCESS on success.
 *          - A non-zero failure code on failure.
 */
int vcdb_builder_add_generic(
    vcdb_builder_t* builder, void* datastore, int type);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus


#endif /*VCDB_BUILDER_PRIVATE_HEADER_GUARD*/
