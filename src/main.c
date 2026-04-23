#include <stdio.h>
#include <stdlib.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_api.h"
#include "lishp/lishp_context.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_runtime.h"
#include "lishp/lishp_value.h"

/*****************************************************************************/
/*                                    REPL                                   */
/*****************************************************************************/

static bool
LISHP_REPL_Body(LISHP_Context* ctx, const char* line)
{
  auto form = LISHP_Read(ctx, line);
  if (form == LISHP_VALUE_INVALID) return true;

  auto repr = LISHP_Write(ctx, form);
  if (!repr) return false;

  printf("=> %s\n", repr);

  return strcmp(line, ":quit") != 0;
}

static void
LISHP_REPL(LISHP_Context* ctx)
{
  bool running = true;
  while (running) {
    auto line = readline("> ");
    if (!line) continue;
    if (*line) add_history(line);

    running = LISHP_REPL_Body(ctx, line);
    free(line);
  }
}

/*****************************************************************************/
/*                                    MAIN                                   */
/*****************************************************************************/

int
main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  auto exit_code = EXIT_SUCCESS;
  auto alloc     = LISHP_GetHeapAlloc();
  auto runtime   = LISHP_Runtime_Create(alloc);
  auto context   = LISHP_Context_Create(runtime);

  if (!runtime || !context) {
    exit_code = EXIT_FAILURE;
    goto exit;
  }

  LISHP_REPL(context);

exit:
  LISHP_Context_Destroy(context);
  LISHP_Runtime_Destroy(runtime);

  return exit_code;
}
