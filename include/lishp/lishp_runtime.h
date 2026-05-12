/**
 * @file lishp_runtime.h
 * @brief Global runtime
 */

#ifndef LISHP_RUNTIME_H
#define LISHP_RUNTIME_H

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_prelude.h"

#include <assert.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

/**
 * @brief Contains the state that is shared across all threads
 */
struct LISHP_Runtime
{
  LISHP_GarbageCollector* garbage_collector;
  LISHP_Allocator*        alloc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

/**
 * @brief Initializer for the global runtime
 */
[[nodiscard]] LISHP_Runtime*
LISHP_Runtime_Initialize(LISHP_Runtime* rt, LISHP_Allocator* alloc);
/**
 * @brief Finalizer for the global runtime
 */
[[nodiscard]] LISHP_Runtime*
LISHP_Runtime_Finalize(LISHP_Runtime* rt);

/**
 * @returns A freshly created global runtime or NULL on failure
 */
[[nodiscard, maybe_unused]] static inline LISHP_Runtime*
LISHP_Runtime_Create(LISHP_Allocator* alloc)
{
  assert(alloc);
  return LISHP_Allocator_Construct(alloc, LISHP_Runtime_Initialize, alloc);
}

/**
 * @brief Destroys the global runtime
 */
[[maybe_unused]] static inline LISHP_Runtime*
LISHP_Runtime_Destroy(LISHP_Runtime* rt)
{
  if (!rt) return nullptr;
  return LISHP_Allocator_Destruct(rt->alloc, LISHP_Runtime_Finalize, rt);
}

/*****************************************************************************/
/*                                 ACCESSORS                                 */
/*****************************************************************************/

/**
 * @returns The runtime's garbage collector
 */
[[nodiscard, maybe_unused]] static inline LISHP_GarbageCollector*
LISHP_Runtime_GetGarbageCollector(LISHP_Runtime* rt)
{
  return rt ? rt->garbage_collector : nullptr;
}

/**
 * @returns The runtime's allocator
 */
[[nodiscard, maybe_unused]] static inline LISHP_Allocator*
LISHP_Runtime_GetAllocator(LISHP_Runtime* rt)
{
  return rt ? rt->alloc : nullptr;
}

#endif /* LISHP_RUNTIME_H */
