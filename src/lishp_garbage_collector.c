#include "lishp/lishp_garbage_collector.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_api.h"
#include "lishp/lishp_object.h"
#include "lishp/lishp_prelude.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_GarbageCollector*
LISHP_GarbageCollector_Initialize(LISHP_GarbageCollector* gc,
                                  LISHP_Allocator*        alloc)
{
  if (!gc) return nullptr;

  constexpr float  GROWTH_FACTOR = 2.0;
  constexpr size_t MIN_THRESHOLD = 1024;
  constexpr size_t MAX_ROOTS     = 128;

  gc->alloc = alloc;

  gc->first_object_header = nullptr;
  gc->last_object_header  = nullptr;

  gc->heap_growth_factor = GROWTH_FACTOR;
  gc->min_heap_threshold = MIN_THRESHOLD;
  gc->heap_threshold     = MIN_THRESHOLD;
  gc->heap_size          = 0;

  gc->roots     = LISHP_Allocator_Malloc(alloc, MAX_ROOTS * sizeof(*gc->roots));
  gc->max_roots = gc->roots ? MAX_ROOTS : 0;
  gc->num_roots = 0;

  if (!gc->roots) return LISHP_GarbageCollector_Destroy(gc);

  return gc;
}

[[nodiscard]] LISHP_GarbageCollector*
LISHP_GarbageCollector_Finalize(LISHP_GarbageCollector* gc)
{
  if (!gc) return nullptr;

  LISHP_GarbageCollector_FreeAllObjects(gc);

  gc->roots     = LISHP_Allocator_Free(gc->alloc, gc->roots);
  gc->max_roots = 0;
  gc->num_roots = 0;

  gc->heap_growth_factor = 0.0;
  gc->min_heap_threshold = 0.0;
  gc->heap_threshold     = 0;
  gc->heap_size          = 0;

  gc->first_object_header = nullptr;
  gc->last_object_header  = nullptr;

  gc->alloc = nullptr;

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

  LISHP_GarbageCollector_TrackObject(gc, h);
  return LISHP_Object_FromHeader(h);
}

void
LISHP_GarbageCollector_Mark(LISHP_GarbageCollector* gc)
{
  assert(gc);

  if (!gc->roots) return;

  for (size_t i = 0; i < gc->num_roots; ++i) {
    LISHP_GarbageCollector_MarkObjectRecursively(gc, gc->roots[i]);
  }
}

void
LISHP_GarbageCollector_Sweep(LISHP_GarbageCollector* gc)
{
  assert(gc);

  LISHP_GarbageCollector_Header* prev = nullptr;
  LISHP_GarbageCollector_Header* cur  = gc->first_object_header;
  LISHP_GarbageCollector_Header* next = nullptr;

  while (cur) {
    next = cur->next;

    auto is_dead  = !cur->marked && !cur->eternal;
    auto is_alive = !is_dead;

    if (is_dead) {
      LISHP_GarbageCollector_UntrackObject(gc, prev, cur, next);
      LISHP_Object_Finalize(LISHP_Object_FromHeader(cur), gc->alloc);
      LISHP_Allocator_Free(gc->alloc, cur);
    }

    if (is_alive) {
      prev        = cur;
      cur->marked = false;
    }

    cur = next;
  }
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

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

void
LISHP_GarbageCollector_TrackObject(LISHP_GarbageCollector*        gc,
                                   LISHP_GarbageCollector_Header* hdr)
{
  assert(gc);
  assert(hdr);

  if (!gc->first_object_header) gc->first_object_header = hdr;
  if (gc->last_object_header) gc->last_object_header->next = hdr;

  gc->last_object_header = hdr;

  auto full_size  = sizeof(*hdr) + hdr->object_size;
  gc->heap_size  += full_size;
}

void
LISHP_GarbageCollector_UntrackObject(LISHP_GarbageCollector*        gc,
                                     LISHP_GarbageCollector_Header* prev,
                                     LISHP_GarbageCollector_Header* cur,
                                     LISHP_GarbageCollector_Header* next)
{
  assert(gc);
  assert(cur);

  // If these were not set, we would be trying to untrack something while
  // nothing was tracked at all.
  assert(gc->first_object_header);
  assert(gc->last_object_header);

  if (prev) prev->next = next;

  if (gc->first_object_header == cur) {
    assert(!prev);
    gc->first_object_header = next;
  }

  if (gc->last_object_header == cur) {
    assert(!next);
    gc->last_object_header = prev;
  }

  auto full_size = sizeof(*cur) + cur->object_size;
  assert(full_size <= gc->heap_size);
  gc->heap_size -= full_size;
}

static bool
LISHP_GarbageCollector_MarkObjectRecursively_function(
  LISHP_Object*          obj,
  [[maybe_unused]] void* ctx)
{
  auto hdr = LISHP_Object_ToHeader(obj);
  if (hdr->marked) return false;

  hdr->marked = true;
  return true;
}

void
LISHP_GarbageCollector_MarkObjectRecursively(LISHP_GarbageCollector* gc,
                                             LISHP_Object*           obj)
{
  assert(gc);
  assert(obj);
  LISHP_Object_Visit(
    obj, LISHP_GarbageCollector_MarkObjectRecursively_function, gc);
}

void
LISHP_GarbageCollector_FreeAllObjects(LISHP_GarbageCollector* gc)
{
  assert(gc);

  auto hdr = gc->first_object_header;
  while (hdr) {
    auto next = hdr->next;
    LISHP_Object_Finalize(LISHP_Object_FromHeader(hdr), gc->alloc);
    LISHP_Allocator_Free(gc->alloc, hdr);
    hdr = next;
  }

  gc->first_object_header = nullptr;
  gc->last_object_header  = nullptr;
  gc->heap_size           = 0;
}
