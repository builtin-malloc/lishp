#include "lishp/lishp_state.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_State*
LISHP_State_Create()
{
  return nullptr;
}

void
LISHP_State_Destroy(LISHP_State* state)
{
  if (!state) return;
}
