#include "lishp/lishp_vm.h"
#include "lishp/lishp_alloc.h"

#include <assert.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_VM*
LISHP_VM_Create(LISHP_Alloc* alloc, const LISHP_Config* config)
{
  assert(alloc);
  assert(config);

  LISHP_VM* vm = LISHP_Alloc_Malloc(alloc, sizeof(*vm));
  if (!vm) {
    // TODO: Print Error
    return nullptr;
  }

  vm->alloc = alloc;

  return vm;
}

void
LISHP_VM_Destroy(LISHP_VM* vm)
{
  if (!vm) return;
  LISHP_Alloc_Free(vm->alloc, vm);
}
