#include "lishp/lishp_config.h"
#include "lishp/lishp_alloc.h"

#include <assert.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Config*
LISHP_Config_Create(LISHP_Allocator* alloc)
{
  assert(alloc);

  LISHP_Config* config = LISHP_Allocator_Malloc(alloc, sizeof(*config));
  if (!config) {
    // TODO: Print Error
    return nullptr;
  }

  config->alloc = alloc;

  return config;
}

void
LISHP_Config_Destroy(LISHP_Config* config)
{
  if (!config) return;
  LISHP_Allocator_Free(config->alloc, config);
}
