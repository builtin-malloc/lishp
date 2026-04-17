#include "lishp/lishp_symtab.h"
#include "lishp/lishp_alloc.h"

#include <assert.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Symtab*
LISHP_Symtab_Create(LISHP_Alloc* alloc)
{
  assert(alloc);

  LISHP_Symtab* symtab = LISHP_Alloc_Malloc(alloc, sizeof(*symtab));
  if (!symtab) {
    // TODO: Print Error
    return nullptr;
  }

  symtab->alloc = alloc;

  return symtab;
}

void
LISHP_Symtab_Destroy(LISHP_Symtab* symtab)
{
  if (!symtab) return;
  LISHP_Alloc_Free(symtab->alloc, symtab);
}
