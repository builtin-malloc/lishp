#include "lishp/lishp_runtime.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Runtime*
LISHP_Runtime_Initialize(LISHP_Runtime* rt, LISHP_Allocator* alloc)
{
  if (!rt) return nullptr;

  rt->alloc = alloc;

  return rt;
}

[[nodiscard]] LISHP_Runtime*
LISHP_Runtime_Finalize(LISHP_Runtime* rt)
{
  if (!rt) return nullptr;

  return rt;
}
