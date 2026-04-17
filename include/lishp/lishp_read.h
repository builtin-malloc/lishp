#ifndef LISHP_READ_H
#define LISHP_READ_H

#include "lishp_alloc.h"
#include "lishp_gc.h"

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_Reader LISHP_Reader;

struct LISHP_Reader
{
  LISHP_Alloc* alloc;
  LISHP_GC*    gc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Reader*
LISHP_Reader_Create(LISHP_Alloc* alloc, LISHP_GC* gc);
void
LISHP_Reader_Destroy(LISHP_Reader* reader);

#endif /* LISHP_READ_H */
