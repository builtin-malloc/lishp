#ifndef LISHP_ALLOC_H
#define LISHP_ALLOC_H

#include <stddef.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef void* (*LISHP_Alloc_MallocFunc)(void* ctx, size_t size);
typedef void  (*LISHP_Alloc_FreeFunc)(void* ctx, void* ptr);
typedef void* (*LISHP_Alloc_ReallocFunc)(void* ctx, void* ptr, size_t size);

typedef struct LISHP_Alloc LISHP_Alloc;

struct LISHP_Alloc
{
  void*                   context;
  LISHP_Alloc_MallocFunc  malloc_func;
  LISHP_Alloc_FreeFunc    free_func;
  LISHP_Alloc_ReallocFunc realloc_func;
};

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] static inline void*
LISHP_Alloc_Malloc(LISHP_Alloc* alloc, size_t size)
{
  if (!alloc) return nullptr;
  if (!alloc->malloc_func) return nullptr;
  if (size == 0) return nullptr;
  return alloc->malloc_func(alloc->context, size);
}

[[maybe_unused]] static inline void*
LISHP_Alloc_Free(LISHP_Alloc* alloc, void* ptr)
{
  if (!alloc) return ptr;
  if (!alloc->free_func) return ptr;
  if (!ptr) return ptr;
  alloc->free_func(alloc->context, ptr);
  return nullptr;
}

[[nodiscard, maybe_unused]] static inline void*
LISHP_Alloc_Realloc(LISHP_Alloc* alloc, void* ptr, size_t size)
{
  if (!alloc) return ptr;
  if (!alloc->realloc_func) return ptr;
  if (size == 0) return ptr;
  if (!ptr) return LISHP_Alloc_Malloc(alloc, size);
  return alloc->realloc_func(alloc->context, ptr, size);
}

/*****************************************************************************/
/*                               HEAP ALLOCATOR                              */
/*****************************************************************************/

[[nodiscard]] LISHP_Alloc*
LISHP_GetHeapAlloc();

#endif /* LISHP_ALLOC_H */
