#include "lishp/lishp_alloc.h"

#include <assert.h>
#include <stdlib.h>

/*****************************************************************************/
/*                               HEAP ALLOCATOR                              */
/*****************************************************************************/

static void*
LISHP_HeapAlloc_Malloc([[maybe_unused]] void* ctx, size_t size)
{
  assert(size > 0);
  return malloc(size);
}

static void
LISHP_HeapAlloc_Free([[maybe_unused]] void* ctx, void* ptr)
{
  assert(ptr);
  free(ptr);
}

static void*
LISHP_HeapAlloc_Realloc([[maybe_unused]] void* ctx, void* ptr, size_t size)
{
  assert(ptr);
  assert(size > 0);
  return realloc(ptr, size);
}

[[nodiscard]] LISHP_Alloc*
LISHP_GetHeapAlloc()
{
  static LISHP_Alloc heap = {
    .context      = nullptr,
    .malloc_func  = LISHP_HeapAlloc_Malloc,
    .free_func    = LISHP_HeapAlloc_Free,
    .realloc_func = LISHP_HeapAlloc_Realloc,
  };
  return &heap;
}
