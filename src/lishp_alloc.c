#include "lishp/lishp_alloc.h"

#include <assert.h>
#include <stdlib.h>

/*****************************************************************************/
/*                             DEFAULT ALLOCATOR                             */
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

const LISHP_Allocator_VirtualTable LISHP_HeapAllocator_virtual_table = {
  .malloc_function  = LISHP_HeapAllocator_Malloc,
  .free_function    = LISHP_HeapAllocator_Free,
  .realloc_function = LISHP_HeapAllocator_Realloc,
};

[[nodiscard]] LISHP_Allocator*
LISHP_HeapAllocator()
{
  static LISHP_Allocator heap_alloc = {
    .virtual_table = &LISHP_HeapAllocator_virtual_table,
    .context       = nullptr,
  };

  return &heap_alloc;
}
