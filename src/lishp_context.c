#include "lishp/lishp_context.h"

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Context*
LISHP_Context_Initialize(LISHP_Context* ctx, LISHP_Runtime* rt)
{
  if (!ctx) return nullptr;

  ctx->runtime = rt;

  return ctx;
}

[[nodiscard]] LISHP_Context*
LISHP_Context_Finalize(LISHP_Context* ctx)
{
  if (!ctx) return nullptr;

  return ctx;
}
