#include "lishp/lishp_alloc.h"

#include <assert.h>
#include <stdlib.h>

/*****************************************************************************/
/*                               HEAP ALLOCATOR                              */
/*****************************************************************************/

static void*
LISHP_HeapAllocator_Malloc([[maybe_unused]] void* ctx, size_t size)
{
  assert(size > 0);
  return malloc(size);
}

static void
LISHP_HeapAllocator_Free([[maybe_unused]] void* ctx, void* ptr)
{
  assert(ptr);
  free(ptr);
}

static void*
LISHP_HeapAllocator_Realloc([[maybe_unused]] void* ctx, void* ptr, size_t size)
{
  assert(ptr);
  assert(size > 0);
  return realloc(ptr, size);
}

[[nodiscard]] LISHP_Allocator*
LISHP_GetHeapAllocator()
{
  static LISHP_Allocator heap = {
    .context      = nullptr,
    .malloc_func  = LISHP_HeapAllocator_Malloc,
    .free_func    = LISHP_HeapAllocator_Free,
    .realloc_func = LISHP_HeapAllocator_Realloc,
  };
  return &heap;
}
