#ifndef LISHP_GARBAGE_COLLECTOR_H
#define LISHP_GARBAGE_COLLECTOR_H

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_object.h"
#include "lishp/lishp_prelude.h"

#include <assert.h>
#include <stdalign.h>
#include <stddef.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_GarbageCollector_Header LISHP_GarbageCollector_Header;

struct LISHP_GarbageCollector
{
  LISHP_Allocator* alloc;

  LISHP_GarbageCollector_Header* first_object_header;
  LISHP_GarbageCollector_Header* last_object_header;

  float  heap_growth_factor;
  size_t min_heap_threshold;
  size_t heap_threshold;
  size_t heap_size;

  size_t         max_roots;
  size_t         num_roots;
  LISHP_Object** roots;
};

struct LISHP_GarbageCollector_Header
{
  size_t                         object_size;
  bool                           marked;
  bool                           eternal;
  LISHP_GarbageCollector_Header* next;
};

static_assert(sizeof(LISHP_Object) % alignof(LISHP_GarbageCollector_Header) ==
              0);

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_GarbageCollector*
LISHP_GarbageCollector_Initialize(LISHP_GarbageCollector* gc,
                                  LISHP_Allocator*        alloc);
[[nodiscard]] LISHP_GarbageCollector*
LISHP_GarbageCollector_Finalize(LISHP_GarbageCollector* gc);

[[nodiscard, maybe_unused]] static inline LISHP_GarbageCollector*
LISHP_GarbageCollector_Create(LISHP_Allocator* alloc)
{
  assert(alloc);
  return LISHP_Allocator_Construct(
    alloc, LISHP_GarbageCollector_Initialize, alloc);
}

[[maybe_unused]] static inline LISHP_GarbageCollector*
LISHP_GarbageCollector_Destroy(LISHP_GarbageCollector* gc)
{
  if (!gc) return nullptr;
  return LISHP_Allocator_Destruct(
    gc->alloc, LISHP_GarbageCollector_Finalize, gc);
}

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] LISHP_Object*
LISHP_GarbageCollector_Malloc(LISHP_GarbageCollector* gc,
                              size_t                  size,
                              LISHP_Context*          ctx);

void
LISHP_GarbageCollector_Mark(LISHP_GarbageCollector* gc);
void
LISHP_GarbageCollector_Sweep(LISHP_GarbageCollector* gc);
void
LISHP_GarbageCollector_RecomputeThreshold(LISHP_GarbageCollector* gc);
[[nodiscard]] bool
LISHP_GarbageCollector_ShouldRun(const LISHP_GarbageCollector* gc);
void
LISHP_GarbageCollector_Run(LISHP_GarbageCollector* gc);

/*****************************************************************************/
/*                                 CONVERSION                                */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Object*
LISHP_Object_FromHeader(LISHP_GarbageCollector_Header* header)
{
  assert(header);
  char*  addr = (void*)header;
  size_t off  = sizeof(LISHP_GarbageCollector_Header);
  return (LISHP_Object*)(addr + off);
}

[[nodiscard, maybe_unused]] static inline LISHP_GarbageCollector_Header*
LISHP_Object_ToHeader(LISHP_Object* object)
{
  assert(object);
  char*  addr = (void*)object;
  size_t off  = sizeof(LISHP_GarbageCollector_Header);
  return (LISHP_GarbageCollector_Header*)(addr - off);
}

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

void
LISHP_GarbageCollector_TrackObject(LISHP_GarbageCollector*        gc,
                                   LISHP_GarbageCollector_Header* hdr);
void
LISHP_GarbageCollector_UntrackObject(LISHP_GarbageCollector*        gc,
                                     LISHP_GarbageCollector_Header* prev,
                                     LISHP_GarbageCollector_Header* cur,
                                     LISHP_GarbageCollector_Header* next);
void
LISHP_GarbageCollector_MarkObjectRecursively(LISHP_GarbageCollector* gc,
                                             LISHP_Object*           obj);

void
LISHP_GarbageCollector_FreeAllObjects(LISHP_GarbageCollector* gc);

#endif /* LISHP_GARBAGE_COLLECTOR_H */
