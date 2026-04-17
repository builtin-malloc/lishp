#ifndef LISHP_ALLOC_H
#define LISHP_ALLOC_H

#include <stddef.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef void* (*LISHP_Allocator_MallocFunction)(void* ctx, size_t size);
typedef void  (*LISHP_Allocator_FreeFunction)(void* ctx, void* ptr);
typedef void* (*LISHP_Allocator_ReallocFunction)(void*  ctx,
                                                 void*  ptr,
                                                 size_t size);

typedef struct LISHP_Allocator LISHP_Allocator;

struct LISHP_Allocator
{
  void*                           context;
  LISHP_Allocator_MallocFunction  malloc_func;
  LISHP_Allocator_FreeFunction    free_func;
  LISHP_Allocator_ReallocFunction realloc_func;
};

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] static inline void*
LISHP_Allocator_Malloc(LISHP_Allocator* alloc, size_t size)
{
  if (!alloc) return nullptr;
  if (!alloc->malloc_func) return nullptr;
  if (size == 0) return nullptr;
  return alloc->malloc_func(alloc->context, size);
}

[[maybe_unused]] static inline void*
LISHP_Allocator_Free(LISHP_Allocator* alloc, void* ptr)
{
  if (!alloc) return ptr;
  if (!alloc->free_func) return ptr;
  if (!ptr) return ptr;
  alloc->free_func(alloc->context, ptr);
  return nullptr;
}

[[nodiscard, maybe_unused]] static inline void*
LISHP_Allocator_Realloc(LISHP_Allocator* alloc, void* ptr, size_t size)
{
  if (!alloc) return ptr;
  if (!alloc->realloc_func) return ptr;
  if (size == 0) return ptr;
  if (!ptr) return LISHP_Allocator_Malloc(alloc, size);
  return alloc->realloc_func(alloc->context, ptr, size);
}

/*****************************************************************************/
/*                               HEAP ALLOCATOR                              */
/*****************************************************************************/

[[nodiscard]] LISHP_Allocator*
LISHP_GetHeapAllocator();

#endif /* LISHP_ALLOC_H */
