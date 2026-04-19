#include "lishp/lishp_symtab.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_diag.h"

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
    LISHP_Diag_WriteError(nullptr, "Failed to allocate global symbol table");
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
