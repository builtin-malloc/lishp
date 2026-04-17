#ifndef LISHP_SYMTAB_H
#define LISHP_SYMTAB_H

#include "lishp_alloc.h"

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_Symtab LISHP_Symtab;

struct LISHP_Symtab
{
  LISHP_Alloc* alloc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Symtab*
LISHP_Symtab_Create(LISHP_Alloc* alloc);
void
LISHP_Symtab_Destroy(LISHP_Symtab* symtab);

#endif /* LISHP_SYMTAB_H */
