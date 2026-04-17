#ifndef LISHP_DIAG_H
#define LISHP_DIAG_H

#include "lishp_alloc.h"
#include "lishp_config.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

typedef struct LISHP_Diag LISHP_Diag;

struct LISHP_Diag
{
  LISHP_Allocator* alloc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Diag*
LISHP_Diag_Create(LISHP_Allocator* alloc, const LISHP_Config* config);
void
LISHP_Diag_Destroy(LISHP_Diag* diag);

#endif /* LISHP_DIAG_H */
