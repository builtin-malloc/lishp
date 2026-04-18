#include "lishp/lishp_runtime.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_config.h"
#include "lishp/lishp_gc.h"
#include "lishp/lishp_symtab.h"

#include <assert.h>
#include <string.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Runtime*
LISHP_Runtime_Create(LISHP_Alloc* alloc)
{
  assert(alloc);

  LISHP_Runtime* rt = LISHP_Alloc_Malloc(alloc, sizeof(*rt));
  if (!rt) goto failure;

  memset(rt, 0, sizeof(*rt));

  rt->config = LISHP_Config_Create(alloc);
  if (!rt->config) goto failure;

  rt->gc = LISHP_GC_Create(alloc, rt->config);
  if (!rt->gc) goto failure;

  rt->symtab = LISHP_Symtab_Create(alloc);
  if (!rt->symtab) goto failure;

  return rt;

failure:
  // TODO: Print Error
  LISHP_Runtime_Destroy(rt);
  return nullptr;
}

void
LISHP_Runtime_Destroy(LISHP_Runtime* runtime)
{
  if (!runtime) return;

  LISHP_Config_Destroy(runtime->config);
  LISHP_GC_Destroy(runtime->gc);
  LISHP_Symtab_Destroy(runtime->symtab);

  LISHP_Alloc_Free(runtime->alloc, runtime);
}
