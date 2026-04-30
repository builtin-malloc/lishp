#include "lishp/lishp_alloc.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_test.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************/
/*                                    MAIN                                   */
/*****************************************************************************/

int
main(int argc, char* argv[])
{
  return LISHP_TestMain(LISHP_HeapAllocator(), argc, argv);
}

/*****************************************************************************/
/*                            MAIN IMPLEMENTATION                            */
/*****************************************************************************/

[[nodiscard]] int
LISHP_TestMain(LISHP_Allocator*       alloc,
               [[maybe_unused]] int   argc,
               [[maybe_unused]] char* argv[])
{
  auto test_ctx = LISHP_TestContext_Create(alloc);
  if (!test_ctx) return EXIT_FAILURE;

  for (auto reg = __start_test_array; reg != __stop_test_array; ++reg) {
    assert(*reg);
    (*reg)(test_ctx);
  }

  LISHP_TestContext_Destroy(test_ctx);
  return EXIT_SUCCESS;
}
