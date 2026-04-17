#include "lishp/lishp_state.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_config.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_State*
LISHP_State_Create(LISHP_Allocator* alloc)
{
  assert(alloc);

  LISHP_State* state = LISHP_Allocator_Malloc(alloc, sizeof(*state));
  if (!state) {
    // TODO: Print Error
    return nullptr;
  }

  state->alloc  = alloc;
  state->config = LISHP_Config_Create(alloc);

  if (!state->config) {
    // TODO: Print Error
    LISHP_State_Destroy(state);
    return nullptr;
  }

  return state;
}

void
LISHP_State_Destroy(LISHP_State* state)
{
  if (!state) return;
  LISHP_Config_Destroy(state->config);
  LISHP_Allocator_Free(state->alloc, state);
}
