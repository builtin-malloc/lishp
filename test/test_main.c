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
  auto exit_code = EXIT_SUCCESS;

  auto registry_begin = __start_test_array;
  auto registry_end   = __stop_test_array;
  auto num_tests      = registry_end - registry_begin;

  auto test_ctx = LISHP_TestContext_Create(alloc, num_tests);
  if (!test_ctx) return EXIT_FAILURE;

  for (auto reg = registry_begin; reg != registry_end; ++reg) {
    assert(*reg);
    (*reg)(test_ctx);
  }

  auto entries_begin = LISHP_TestContext_EntriesBegin(test_ctx);
  auto entries_end   = LISHP_TestContext_EntriesEnd(test_ctx);
  for (auto entry = entries_begin; entry != entries_end; ++entry) {
    assert(entry->func);
    entry->func(test_ctx, entry, nullptr);
  }

  if (LISHP_TestContext_HasAnyFailures(test_ctx)) {
    exit_code = EXIT_FAILURE;
    LISHP_TestContext_PrintFailures(test_ctx, stderr);
  }

  LISHP_TestContext_PrintSummary(test_ctx, stdout);

  LISHP_TestContext_Destroy(test_ctx);
  return exit_code;
}
