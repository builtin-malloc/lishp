#ifndef LISHP_VM_H
#define LISHP_VM_H

#include "lishp_alloc.h"
#include "lishp_config.h"

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_VM LISHP_VM;

struct LISHP_VM
{
  LISHP_Alloc* alloc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_VM*
LISHP_VM_Create(LISHP_Alloc* alloc, const LISHP_Config* config);
void
LISHP_VM_Destroy(LISHP_VM* vm);

#endif /* LISHP_VM_H */
