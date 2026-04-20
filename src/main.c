#include <stdio.h>
#include <stdlib.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_context.h"
#include "lishp/lishp_runtime.h"

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

  bool running = true;
  while (running) {
    auto line = readline("> ");
    if (!line) continue;

    if (*line) {
      add_history(line);
    }

    running = strcmp(line, ":quit") != 0;

    printf("=> %s\n", line);

    free(line);
  }

exit:
  LISHP_Context_Destroy(context);
  LISHP_Runtime_Destroy(runtime);

  return exit_code;
}
