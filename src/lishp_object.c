#include "lishp/lishp_object.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_hash.h"

#include <assert.h>
#include <string.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

void
LISHP_Object_InitializeNil(LISHP_Object*                   obj,
                           [[maybe_unused]] LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);

  obj->tag = LISHP_OBJECTTAG_NIL;
}

void
LISHP_Object_InitializeCons(LISHP_Object*                   obj,
                            LISHP_Value                     car,
                            LISHP_Value                     cdr,
                            [[maybe_unused]] LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);

  obj->as_cons.tag = LISHP_OBJECTTAG_CONS;
  obj->as_cons.car = car;
  obj->as_cons.cdr = cdr;
}

void
LISHP_Object_InitializeBool(LISHP_Object*                   obj,
                            bool                            value,
                            [[maybe_unused]] LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);

  obj->as_bool.tag   = LISHP_OBJECTTAG_BOOL;
  obj->as_bool.value = value;
}

void
LISHP_Object_InitializeChar(LISHP_Object*                   obj,
                            char                            value,
                            [[maybe_unused]] LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);

  obj->as_char.tag   = LISHP_OBJECTTAG_CHAR;
  obj->as_char.value = value;
}

void
LISHP_Object_InitializeFloat(LISHP_Object*                   obj,
                             double                          value,
                             [[maybe_unused]] LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);

  obj->as_float.tag   = LISHP_OBJECTTAG_FLOAT;
  obj->as_float.value = value;
}

void
LISHP_Object_InitializeBigint(LISHP_Object*                   obj,
                              int8_t                          sign,
                              size_t                          num_legs,
                              const uint32_t*                 legs,
                              [[maybe_unused]] LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);

  obj->as_bigint.tag      = LISHP_OBJECTTAG_BIGINT;
  obj->as_bigint.sign     = sign;
  obj->as_bigint.num_legs = num_legs;
  memcpy(obj->as_bigint.legs, legs, num_legs * sizeof(*legs));
}

void
LISHP_Object_InitializeRatio(LISHP_Object*                   obj,
                             LISHP_Value                     numerator,
                             LISHP_Value                     denominator,
                             [[maybe_unused]] LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);

  obj->as_ratio.tag         = LISHP_OBJECTTAG_RATIO;
  obj->as_ratio.numerator   = numerator;
  obj->as_ratio.denominator = denominator;
}

static void
LISHP_Object_InitializeStringlike(struct LISHP_Object_Stringlike* obj,
                                  LISHP_ObjectTag                 tag,
                                  size_t                          len,
                                  const char*                     data,
                                  [[maybe_unused]] LISHP_Context* ctx)
{
  obj->tag    = tag;
  obj->hash   = LISHP_Hash_fnv1a(len, data);
  obj->length = len;
  memcpy(obj->data, data, len);
  obj->data[len] = '\0';
}

void
LISHP_Object_InitializeSymbol(LISHP_Object*  obj,
                              size_t         len,
                              const char*    data,
                              LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);
  LISHP_Object_InitializeStringlike(
    &obj->as_symbol, LISHP_OBJECTTAG_SYMBOL, len, data, ctx);
}

void
LISHP_Object_InitializeKeyword(LISHP_Object*  obj,
                               size_t         len,
                               const char*    data,
                               LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);
  LISHP_Object_InitializeStringlike(
    &obj->as_keyword, LISHP_OBJECTTAG_KEYWORD, len, data, ctx);
}

void
LISHP_Object_InitializeString(LISHP_Object*  obj,
                              size_t         len,
                              const char*    data,
                              LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);
  LISHP_Object_InitializeStringlike(
    &obj->as_string, LISHP_OBJECTTAG_STRING, len, data, ctx);
}

void
LISHP_Object_Finalize(LISHP_Object* obj, LISHP_Context* ctx)
{
  assert(obj);
  assert(ctx);
}
