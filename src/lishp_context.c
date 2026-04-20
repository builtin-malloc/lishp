#include "lishp/lishp_context.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_diag.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_read.h"
#include "lishp/lishp_runtime.h"
#include "lishp/lishp_vm.h"
#include "lishp/lishp_write.h"

#include <string.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Context*
LISHP_Context_Create(LISHP_Runtime* runtime)
{
  if (!runtime) return nullptr;

  auto alloc  = LISHP_Runtime_GetAlloc(runtime);
  auto config = LISHP_Runtime_GetConfig(runtime);
  auto gc     = LISHP_Runtime_GetGC(runtime);

  LISHP_Context* ctx = LISHP_Alloc_Malloc(alloc, sizeof(*ctx));
  if (!ctx) goto failure;

  memset(ctx, 0, sizeof(*ctx));

  ctx->runtime = runtime;

  ctx->diag = LISHP_Diag_Create(alloc, config);
  if (!ctx->diag) goto failure;

  ctx->vm = LISHP_VM_Create(alloc, config);
  if (!ctx->vm) goto failure;

  ctx->reader = LISHP_Reader_Create(alloc, gc);
  if (!ctx->reader) goto failure;

  ctx->writer = LISHP_Writer_Create(alloc);
  if (!ctx->writer) goto failure;

  return ctx;

failure:
  LISHP_Diag_WriteFatal(ctx ? ctx->diag : nullptr,
                        "Failed to allocate thread-local context");
  LISHP_Context_Destroy(ctx);
  return nullptr;
}

void
LISHP_Context_Destroy(LISHP_Context* ctx)
{
  if (!ctx) return;

  LISHP_Diag_Destroy(ctx->diag);
  LISHP_VM_Destroy(ctx->vm);
  LISHP_Reader_Destroy(ctx->reader);
  LISHP_Writer_Destroy(ctx->writer);

  LISHP_Alloc_Free(LISHP_Context_GetAlloc(ctx), ctx);
}
