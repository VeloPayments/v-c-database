/**
 * \file vcdb_builder_init.c
 *
 * \brief Implementation of the vcdb_builder_init method.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vcdb/builder.h>
#include <vpr/parameters.h>

/* forward decls */
static void vcdb_builder_dispose(void* disposable);

/**
 * \brief Initialize a builder instance from an engine string and a connection
 * string.
 *
 * The engine string connects to a particular database engine that was
 * previously registered.  The connection string is used to resolve a particular
 * database instance and is implementation-specific.
 *
 * \param builder   The builder instance to initialize.
 * \param engine    The database engine to use.
 * \param connect   The implementation-dependent connection string for a
 *                  database instance.
 *
 * \returns A status code signifying success or failure.
 *          * VCDB_STATUS_SUCCESS on success.
 *          * a non-zero failure code on failure.
 */
int vcdb_builder_init(
    vcdb_builder_t* builder,
    const char* engine,
    const char* connect)
{
    MODEL_ASSERT(NULL != builder);
    MODEL_ASSERT(NULL != engine);
    MODEL_ASSERT(NULL != connect);

    /* parameter check */
    if (NULL == builder || NULL == engine || NULL == connect)
    {
        return VCDB_ERROR_INVALID_PARAMETER;
    }

    /* look up the database engine. */
    vcdb_database_engine_t* eng = vcdb_database_engine_lookup(engine);
    if (NULL == eng)
    {
        return VCDB_ERROR_MISSING_DATABASE_ENGINE;
    }

    /* set up the builder instance. */
    builder->hdr.dispose = &vcdb_builder_dispose;
    builder->engine = eng;
    builder->instance_array_max = DEFAULT_INSTANCE_SIZE;
    builder->instance_array_size = 0;
    builder->database_opened = false;

    /* attempt to duplicate the connection string. */
    builder->connection_string = strdup(connect);
    if (NULL == builder->connection_string)
    {
        return VCDB_ERROR_BAD_MEMORY_ALLOCATION;
    }

    MODEL_ASSERT(NULL != builder->connection_string);

    /* attempt to allocate the instance array. */
    builder->instance_array = (vcdb_builder_datastore_instance_t*)
        malloc(sizeof(vcdb_builder_datastore_instance_t) * builder->instance_array_max);
    if (NULL == builder->instance_array)
    {
        free(builder->connection_string);

        return VCDB_ERROR_BAD_MEMORY_ALLOCATION;
    }

    MODEL_ASSERT(NULL != builder->instance_array);

    /* clear the array. */
    memset(builder->instance_array,
        0,
        sizeof(vcdb_builder_datastore_instance_t) * builder->instance_array_max);

    return VCDB_STATUS_SUCCESS;
}

/**
 * \brief Clean up the builder instance.
 *
 * \param disposable        The builder instance to be disposed.
 */
static void vcdb_builder_dispose(void* disposable)
{
    vcdb_builder_t* builder = (vcdb_builder_t*)disposable;

    MODEL_ASSERT(NULL != builder);
    MODEL_ASSERT(NULL != builder->connection_string);
    MODEL_ASSERT(NULL != builder->instance_array);
    MODEL_ASSERT(0 <= builder->instance_array_size);
    MODEL_ASSERT(!builder->database_opened);

    /* clean up the connection string. */
    free(builder->connection_string);

    /* clean up each active instance */
    for (size_t i = 0; i < builder->instance_array_size; ++i)
    {
        /* both datastore and index HAS-A disposable_t as a first member, so
         * this cast is safe.
         */
        dispose((disposable_t*)&builder->instance_array[i].instance);
    }

    /* clean up the instance array. */
    free(builder->instance_array);

    /* clear out the structure. */
    memset(builder, 0, sizeof(vcdb_builder_t));
}
