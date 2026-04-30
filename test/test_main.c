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
  auto registry_begin = __start_test_array;
  auto registry_end   = __stop_test_array;
  auto num_tests      = registry_end - registry_begin;

  auto test_ctx = LISHP_TestContext_Create(alloc, num_tests);
  if (!test_ctx) return EXIT_FAILURE;

  for (auto reg = registry_begin; reg != registry_end; ++reg) {
    assert(*reg);
    (*reg)(test_ctx);
  }

LISHP_TestContext_Destroy(test_ctx);
  return EXIT_SUCCESS;
}
