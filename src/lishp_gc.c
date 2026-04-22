#include "lishp/lishp_gc.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_diag.h"

#include <assert.h>
#include <stdlib.h>

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

  gc->alloc = alloc;

  return gc;
}

void
LISHP_GC_Destroy(LISHP_GC* gc)
{
  if (!gc) return;
  LISHP_Alloc_Free(gc->alloc, gc);
}

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] LISHP_Object*
LISHP_GC_Malloc(LISHP_GC* gc, size_t size, LISHP_Context* ctx)
{
  assert(gc);
  assert(ctx);
  assert(size > 0);

  size_t total_size = sizeof(LISHP_GC_Header) + size;

  LISHP_GC_Header* header = LISHP_Alloc_Malloc(gc->alloc, total_size);
  if (!header) {
    LISHP_GC_Run(gc, ctx);
    header = LISHP_Alloc_Malloc(gc->alloc, total_size);
    if (!header) {
      // TODO: Raise error condition via ctx
      abort();
    }
  }

  header->size  = total_size;
  header->state = LISHP_GC_HEADERSTATE_UNMARKED;
  header->next  = nullptr;

  LISHP_GC_TrackObject(gc, header, ctx);
  return LISHP_GC_Header_ToObject(header);
}

void
LISHP_GC_Run(LISHP_GC* gc, LISHP_Context* ctx)
{
  assert(gc);
  assert(ctx);
}

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

void
LISHP_GC_TrackObject(LISHP_GC* gc, LISHP_GC_Header* header, LISHP_Context* ctx)
{
  assert(gc);
  assert(header);
  assert(ctx);
}

void
LISHP_GC_UntrackObject(LISHP_GC*        gc,
                       LISHP_GC_Header* header,
                       LISHP_Context*   ctx)
{
  assert(gc);
  assert(header);
  assert(ctx);
}
