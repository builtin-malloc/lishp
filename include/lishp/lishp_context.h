/**
 * @file lishp_context.h
 * @brief Local context
 */

#ifndef LISHP_CONTEXT_H
#define LISHP_CONTEXT_H

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_runtime.h"

#include <assert.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

/**
 * @brief Contains the state that is local to each thread
 */
struct LISHP_Context
{
  LISHP_Runtime* runtime;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

/**
 * @brief Initializer for the global context
 */
[[nodiscard]] LISHP_Context*
LISHP_Context_Initialize(LISHP_Context* ctx, LISHP_Runtime* rt);
/**
 * @brief Finalizer for the global context
 */
[[nodiscard]] LISHP_Context*
LISHP_Context_Finalize(LISHP_Context* ctx);

/**
 * @returns A freshly created global context or NULL on failure
 */
[[nodiscard, maybe_unused]] static inline LISHP_Context*
LISHP_Context_Create(LISHP_Runtime* rt)
{
  if (!rt) return nullptr;

  auto alloc = LISHP_Runtime_GetAllocator(rt);

  return LISHP_Allocator_Construct(alloc, LISHP_Context_Initialize, rt);
}

/**
 * @brief Destroys the global context
 */
[[maybe_unused]] static inline LISHP_Context*
LISHP_Context_Destroy(LISHP_Context* ctx)
{
  if (!ctx) return nullptr;

  auto alloc = LISHP_Runtime_GetAllocator(ctx->runtime);

  return LISHP_Allocator_Destruct(alloc, LISHP_Context_Finalize, ctx);
}

/*****************************************************************************/
/*                                 ACCESSORS                                 */
/*****************************************************************************/

/**
 * @returns The context's garbage collector
 * @note It is inherited from the global runtime used to create this context
 */
[[nodiscard, maybe_unused]] static inline LISHP_GarbageCollector*
LISHP_Context_GetGarbageCollector(LISHP_Context* ctx)
{
  return ctx ? LISHP_Runtime_GetGarbageCollector(ctx->runtime) : nullptr;
}

/**
 * @returns The context's allocator
 * @note It is inherited from the global runtime used to create this context
 */
[[nodiscard, maybe_unused]] static inline LISHP_Allocator*
LISHP_Context_GetAllocator(LISHP_Context* ctx)
{
  return ctx ? LISHP_Runtime_GetAllocator(ctx->runtime) : nullptr;
}

#endif /* LISHP_CONTEXT_H */
