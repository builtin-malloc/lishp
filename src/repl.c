#include "repl.h"

#include "lishp/lishp_api.h"
#include "lishp/lishp_context.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_runtime.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/history.h>
#include <readline/readline.h>

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

static bool
repl_body(LISHP_Context* ctx, const char* src)
{
  assert(ctx);

  auto form = LISHP_Read(ctx, src);
  form      = LISHP_Expand(ctx, form);
  form      = LISHP_Evaluate(ctx, form);
  auto repr = LISHP_Print(ctx, form);

  if (repr) printf("\n=> %s\n\n", repr);

  if (strcmp(src, ":quit") == 0) return false;
  return true;
}

static int
repl_loop(LISHP_Context* ctx)
{
  assert(ctx);

  bool running = true;
  while (running) {
    char* line = readline("> ");
    if (!line) return EXIT_FAILURE;

    if (*line) {
      add_history(line);
      running = repl_body(ctx, line);
    }

    free(line);
  }

  return EXIT_SUCCESS;
}

[[nodiscard]] int
repl(LISHP_Allocator* alloc)
{
  assert(alloc);

  auto rt  = LISHP_Runtime_Create(alloc);
  auto ctx = LISHP_Context_Create(rt);

  if (!rt || !ctx) return EXIT_FAILURE;

  auto exit_code = repl_loop(ctx);

  LISHP_Context_Destroy(ctx);
  LISHP_Runtime_Destroy(rt);

  return exit_code;
}
