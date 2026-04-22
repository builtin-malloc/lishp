#ifndef LISHP_GC_H
#define LISHP_GC_H

#include "lishp_alloc.h"
#include "lishp_config.h"
#include "lishp_object.h"
#include "lishp_prelude.h"

#include <assert.h>
#include <stdalign.h>
#include <stddef.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_GC        LISHP_GC;
typedef struct LISHP_GC_Header LISHP_GC_Header;

struct LISHP_GC
{
  LISHP_Alloc* alloc;
};

typedef enum LISHP_GC_HeaderState
{
  LISHP_GC_HEADERSTATE_UNMARKED,
  LISHP_GC_HEADERSTATE_MARKED,
  LISHP_GC_HEADERSTATE_ETERNAL,
} LISHP_GC_HeaderState;

struct LISHP_GC_Header
{
  size_t               size;
  LISHP_GC_HeaderState state;
  LISHP_GC_Header*     next;
};

static_assert(sizeof(LISHP_GC_Header) % alignof(LISHP_Object) == 0);

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_GC*
LISHP_GC_Create(LISHP_Alloc* alloc, const LISHP_Config* config);
void
LISHP_GC_Destroy(LISHP_GC* gc);

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] LISHP_Object*
LISHP_GC_Malloc(LISHP_GC* gc, size_t size, LISHP_Context* ctx);

void
LISHP_GC_Run(LISHP_GC* gc, LISHP_Context* ctx);

/*****************************************************************************/
/*                                 CONVERSION                                */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Object*
LISHP_GC_Header_ToObject(LISHP_GC_Header* header)
{
  assert(header);
  return (LISHP_Object*)(((char*)header) + sizeof(LISHP_GC_Header));
}

[[nodiscard, maybe_unused]] static inline LISHP_GC_Header*
LISHP_GC_Header_FromObject(LISHP_Object* object)
{
  assert(object);
  return (LISHP_GC_Header*)(((char*)object) - sizeof(LISHP_GC_Header));
}

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

void
LISHP_GC_TrackObject(LISHP_GC* gc, LISHP_GC_Header* header, LISHP_Context* ctx);
void
LISHP_GC_UntrackObject(LISHP_GC*        gc,
                       LISHP_GC_Header* header,
                       LISHP_Context*   ctx);

#endif /* LISHP_GC_H */
