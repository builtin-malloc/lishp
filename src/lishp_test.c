#include "lishp/lishp_test.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_TestContext*
LISHP_TestContext_Initialize(LISHP_TestContext* ctx, LISHP_Allocator* alloc)
{
  if (!ctx) return nullptr;

  ctx->alloc = alloc;

  return ctx;
}

[[nodiscard]] LISHP_TestContext*
LISHP_TestContext_Finalize(LISHP_TestContext* ctx)
{
  if (!ctx) return nullptr;

  return ctx;
}
