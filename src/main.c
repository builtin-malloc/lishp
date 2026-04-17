#include <stdlib.h>

#include "lishp/lishp_state.h"

int
main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  auto state = LISHP_State_Create();
  if (!state) return EXIT_FAILURE;

  LISHP_State_Destroy(state);
  return EXIT_SUCCESS;
}
