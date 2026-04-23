#ifndef LISHP_API_H
#define LISHP_API_H

#include "lishp_context.h"
#include "lishp_prelude.h"
#include "lishp_read.h"
#include "lishp_write.h"

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

[[nodiscard, maybe_unused]] static inline const char*
LISHP_Write(LISHP_Context* ctx, LISHP_Value value)
{
  assert(ctx);
  auto mode = LISHP_WRITEMODE_RAW;
  return LISHP_Writer_Write(LISHP_Context_GetWriter(ctx), value, mode, ctx);
}

/*****************************************************************************/
/*                                  OBJECTS                                  */
/*****************************************************************************/

[[nodiscard]] LISHP_Value
LISHP_CreateNil(LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateCons(LISHP_Value car, LISHP_Value cdr, LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateBool(bool value, LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateChar(char value, LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateFloat(double value, LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateBigint(int             sign,
                   size_t          num_legs,
                   const uint32_t* legs,
                   LISHP_Context*  ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateRatio(LISHP_Value    numerator,
                  LISHP_Value    denominator,
                  LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateSymbol(size_t len, const char* data, LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateKeyword(size_t len, const char* data, LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_CreateString(size_t len, const char* data, LISHP_Context* ctx);

#endif /* LISHP_API_H */
