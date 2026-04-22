#ifndef LISHP_API_H
#define LISHP_API_H

#include "lishp_context.h"
#include "lishp_prelude.h"
#include "lishp_read.h"

#include <assert.h>

/*****************************************************************************/
/*                                 READ/WRITE                                */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Value
LISHP_Read(LISHP_Context* ctx, const char* src)
{
  assert(ctx);
  return LISHP_Reader_Read(LISHP_Context_GetReader(ctx), src, ctx);
}

#endif /* LISHP_API_H */
