#include "lishp/lishp_state.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_config.h"
#include "lishp/lishp_diag.h"

#include <string.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_State*
LISHP_State_Create(LISHP_Alloc* alloc)
{
  assert(alloc);

  LISHP_State* state = LISHP_Alloc_Malloc(alloc, sizeof(*state));
  if (!state) {
    // TODO: Print Error
    return nullptr;
  }

  memset(state, 0, sizeof(*state));

  state->alloc = alloc;

  state->config = LISHP_Config_Create(alloc);
  if (!state->config) goto alloc_failure;

  state->diag = LISHP_Diag_Create(alloc, state->config);
  if (!state->diag) goto alloc_failure;

  return state;

alloc_failure:
  // TODO: Print Error
  LISHP_State_Destroy(state);
  return nullptr;
}

void
LISHP_State_Destroy(LISHP_State* state)
{
  if (!state) return;
  LISHP_Diag_Destroy(state->diag);
  LISHP_Config_Destroy(state->config);
  LISHP_Alloc_Free(state->alloc, state);
}
