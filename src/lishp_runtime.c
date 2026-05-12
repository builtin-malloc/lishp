#include "lishp/lishp_runtime.h"
#include "lishp/lishp_garbage_collector.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Runtime*
LISHP_Runtime_Initialize(LISHP_Runtime* rt, LISHP_Allocator* alloc)
{
  if (!rt) return nullptr;

  rt->garbage_collector = LISHP_GarbageCollector_Create(alloc);
  rt->alloc             = alloc;

  if (!rt->garbage_collector) return LISHP_Runtime_Destroy(rt);

  return rt;
}

[[nodiscard]] LISHP_Runtime*
LISHP_Runtime_Finalize(LISHP_Runtime* rt)
{
  if (!rt) return nullptr;

  rt->garbage_collector = LISHP_GarbageCollector_Destroy(rt->garbage_collector);

  return rt;
}
