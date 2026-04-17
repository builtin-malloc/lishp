#ifndef LISHP_GC_H
#define LISHP_GC_H

#include "lishp_alloc.h"
#include "lishp_config.h"

#include <assert.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_GC LISHP_GC;

struct LISHP_GC
{
  LISHP_Alloc* alloc;
  LISHP_Alloc  as_alloc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_GC*
LISHP_GC_Create(LISHP_Alloc* alloc, const LISHP_Config* config);
void
LISHP_GC_Destroy(LISHP_GC* gc);

/*****************************************************************************/
/*                                 ACCESSORS                                 */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Alloc*
LISHP_GC_AsAlloc(LISHP_GC* gc)
{
  assert(gc);
  return &gc->as_alloc;
}

#endif /* LISHP_GC_H */
