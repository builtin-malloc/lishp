#include "lishp/lishp_garbage_collector.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_api.h"
#include "lishp/lishp_prelude.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_GarbageCollector*
LISHP_GarbageCollector_Initialize(LISHP_GarbageCollector* gc,
                                  LISHP_Allocator*        alloc)
{
  if (!gc) return nullptr;

  gc->alloc = alloc;

  gc->first_object = nullptr;
  gc->last_object  = nullptr;

  gc->heap_growth_factor = 2.0;
  gc->min_heap_threshold = 1024;
  gc->heap_threshold     = gc->min_heap_threshold;
  gc->heap_size          = 0;

  return gc;
}

[[nodiscard]] LISHP_GarbageCollector*
LISHP_GarbageCollector_Finalize(LISHP_GarbageCollector* gc)
{
  if (!gc) return nullptr;

  return gc;
}

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] LISHP_Object*
LISHP_GarbageCollector_Malloc(LISHP_GarbageCollector* gc,
                              size_t                  size,
                              LISHP_Context*          ctx)
{
  assert(gc);
  assert(size >= sizeof(LISHP_Object));
  assert(ctx);

  if (LISHP_GarbageCollector_ShouldRun(gc)) LISHP_GarbageCollector_Run(gc);

  auto alloc      = gc->alloc;
  auto total_size = size + sizeof(LISHP_GarbageCollector_Header);

  LISHP_GarbageCollector_Header* h = LISHP_Allocator_Malloc(alloc, total_size);
  if (!h) {
    LISHP_GarbageCollector_Run(gc);
    h = LISHP_Allocator_Malloc(alloc, total_size);

    if (!h) {
      auto cond   = LISHP_OUT_OF_MEMORY_SYMBOL;
      auto detail = LISHP_NIL;
      LISHP_Error(ctx, cond, detail);
    }
  }

  h->object_size = size;
  h->marked      = false;
  h->eternal     = false;
  h->next        = nullptr;

  return LISHP_Object_FromHeader(h);
}

void
LISHP_GarbageCollector_Mark(LISHP_GarbageCollector* gc)
{
  assert(gc);
}

void
LISHP_GarbageCollector_Sweep(LISHP_GarbageCollector* gc)
{
  assert(gc);
}

void
LISHP_GarbageCollector_RecomputeThreshold(LISHP_GarbageCollector* gc)
{
  assert(gc);

  gc->heap_threshold = gc->heap_growth_factor * gc->heap_size;
  if (gc->heap_threshold < gc->min_heap_threshold) {
    gc->heap_threshold = gc->min_heap_threshold;
  }
}

[[nodiscard]] bool
LISHP_GarbageCollector_ShouldRun(const LISHP_GarbageCollector* gc)
{
  assert(gc);
  return gc->heap_size >= gc->heap_threshold;
}

 void
LISHP_GarbageCollector_Run(LISHP_GarbageCollector* gc)
{
  assert(gc);

  LISHP_GarbageCollector_Mark(gc);
  LISHP_GarbageCollector_Sweep(gc);
  LISHP_GarbageCollector_RecomputeThreshold(gc);
}

