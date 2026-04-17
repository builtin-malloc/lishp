#include "lishp/lishp_diag.h"
#include "lishp/lishp_alloc.h"

#include <assert.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Diag*
LISHP_Diag_Create(LISHP_Alloc* alloc, const LISHP_Config* config)
{
  assert(alloc);
  assert(config);

  LISHP_Diag* diag = LISHP_Alloc_Malloc(alloc, sizeof(*diag));
  if (!diag) {
    // TODO: Print error
    return nullptr;
  }

  diag->alloc = alloc;

  return diag;
}

void
LISHP_Diag_Destroy(LISHP_Diag* diag)
{
  if (!diag) return;
  LISHP_Alloc_Free(diag->alloc, diag);
}
