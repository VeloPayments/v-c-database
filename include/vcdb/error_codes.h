/**
 * \file error_codes.h
 *
 * \brief Error codes for vcdb.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VCDB_ERROR_CODES_HEADER_GUARD
#define VCDB_ERROR_CODES_HEADER_GUARD

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \defgroup VCDBErrorCodes Error codes for the Velo C Database Library.
 *
 * \brief These status and error codes are returned by VCDB functions.
 *
 * @{
 */

/**
 * \brief The VCDB_STATUS_SUCCESS code represents the successful completion of a
 * VCDB method.
 */
#define VCDB_STATUS_SUCCESS 0x0000

/**
 * \brief A parameter provided to a vcdb method was invalid.
 */
#define VCDB_ERROR_INVALID_PARAMETER 0x4001

/**
 * @}
 */

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VCDB_ERROR_CODES_HEADER_GUARD
