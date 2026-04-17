#ifndef LISHP_WRITE_H
#define LISHP_WRITE_H

#include "lishp_alloc.h"
#include "lishp_gc.h"

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_Writer LISHP_Writer;

struct LISHP_Writer
{
  LISHP_Alloc* alloc;
  LISHP_GC*    gc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Writer*
LISHP_Writer_Create(LISHP_Alloc* alloc);
void
LISHP_Writer_Destroy(LISHP_Writer* writer);

#endif /* LISHP_WRITE_H */
