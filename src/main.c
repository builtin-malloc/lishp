#include <stdlib.h>

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_runtime.h"
#include "lishp/lishp_state.h"

int
main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  auto runtime = LISHP_Runtime_Create(LISHP_GetHeapAlloc());
  if (!runtime) return EXIT_FAILURE;
  auto state = LISHP_State_Create(LISHP_GetHeapAlloc());
  if (!state) return EXIT_FAILURE;

  LISHP_Runtime_Destroy(runtime);
  LISHP_State_Destroy(state);
  return EXIT_SUCCESS;
}
