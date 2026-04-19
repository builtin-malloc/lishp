#include "lishp/lishp_gc.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_diag.h"

#include <assert.h>

/*****************************************************************************/
/*                            ALLOC IMPLEMENTATION                           */
/*****************************************************************************/

static void*
LISHP_GC_Malloc(void* ctx, size_t size)
{
  assert(ctx);
  assert(size > 0);
  // TODO: Implement this
  return nullptr;
}

static void*
LISHP_GC_Realloc(void* ctx, void* ptr, size_t size)
{
  assert(ctx);
  assert(ptr);
  assert(size > 0);
  // TODO: Implement this
  return nullptr;
}

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_GC*
LISHP_GC_Create(LISHP_Alloc* alloc, const LISHP_Config* config)
{
  assert(alloc);
  assert(config);

  LISHP_GC* gc = LISHP_Alloc_Malloc(alloc, sizeof(*gc));
  if (!gc) {
    LISHP_Diag_WriteError(nullptr, "Failed to allocate garbage collector");
    return nullptr;
  }

  gc->alloc                 = alloc;
  gc->as_alloc.context      = gc;
  gc->as_alloc.malloc_func  = LISHP_GC_Malloc;
  gc->as_alloc.free_func    = nullptr;
  gc->as_alloc.realloc_func = LISHP_GC_Realloc;

  return gc;
}

void
LISHP_GC_Destroy(LISHP_GC* gc)
{
  if (!gc) return;
  LISHP_Alloc_Free(gc->alloc, gc);
}
