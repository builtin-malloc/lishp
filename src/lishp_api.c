#include "lishp/lishp_api.h"
#include "lishp/lishp_context.h"
#include "lishp/lishp_gc.h"
#include "lishp/lishp_object.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_value.h"

#include <assert.h>

/*****************************************************************************/
/*                                  OBJECTS                                  */
/*****************************************************************************/

[[nodiscard]] LISHP_Value
LISHP_CreateNil(LISHP_Context* ctx)
{
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_ObjectTag);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeNil(obj, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateCons(LISHP_Value car, LISHP_Value cdr, LISHP_Context* ctx)
{
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Cons);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeCons(obj, car, cdr, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateBool(bool value, LISHP_Context* ctx)
{
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Bool);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeBool(obj, value, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateChar(char value, LISHP_Context* ctx)
{
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Char);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeChar(obj, value, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateFloat(double value, LISHP_Context* ctx)
{
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Float);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeFloat(obj, value, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateBigint(size_t num_legs, const uint32_t* legs, LISHP_Context* ctx)
{
  assert(num_legs > 0);
  assert(legs);
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Bigint) + num_legs * sizeof(*legs);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeBigint(obj, num_legs, legs, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateRatio(LISHP_Value    numerator,
                  LISHP_Value    denominator,
                  LISHP_Context* ctx)
{
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Ratio);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeRatio(obj, numerator, denominator, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateSymbol(size_t len, const char* data, LISHP_Context* ctx)
{
  assert(data);
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Symbol) + (len + 1) * sizeof(*data);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeSymbol(obj, len, data, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateKeyword(size_t len, const char* data, LISHP_Context* ctx)
{
  assert(data);
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Keyword) + (len + 1) * sizeof(*data);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeKeyword(obj, len, data, ctx);

  return LISHP_Value_MakeBoxed(obj);
}

[[nodiscard]] LISHP_Value
LISHP_CreateString(size_t len, const char* data, LISHP_Context* ctx)
{
  assert(data);
  assert(ctx);

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_String) + (len + 1) * sizeof(*data);
  auto obj  = LISHP_GC_Malloc(gc, size, ctx);

  assert(obj);
  LISHP_Object_InitializeString(obj, len, data, ctx);

  return LISHP_Value_MakeBoxed(obj);
}
