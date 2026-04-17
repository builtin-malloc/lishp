#include "lishp/lishp_config.h"
#include "lishp/lishp_alloc.h"

#include <assert.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Config*
LISHP_Config_Create(LISHP_Alloc* alloc)
{
  assert(alloc);

  LISHP_Config* config = LISHP_Alloc_Malloc(alloc, sizeof(*config));
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
  LISHP_Alloc_Free(config->alloc, config);
}
