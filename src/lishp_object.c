#include "lishp/lishp_object.h"
#include "lishp/lishp_hash.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_value.h"

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

/*****************************************************************************/
/*                                 OPERATIONS                                */
/*****************************************************************************/

static bool
LISHP_Object_BigintAreEqual(const LISHP_Object_Bigint* a,
                            const LISHP_Object_Bigint* b)
{
  if (a->num_legs != b->num_legs) return false;
  return memcmp(a->legs, b->legs, a->num_legs * sizeof(*a->legs)) == 0;
}

static bool
LISHP_Object_RatioAreEqual(const LISHP_Object_Ratio* a,
                           const LISHP_Object_Ratio* b)
{
  return LISHP_Value_AreEqual(a->numerator, b->numerator) &&
         LISHP_Value_AreEqual(a->denominator, b->denominator);
}

static bool
LISHP_Object_StringlikeAreEqual(const struct LISHP_Object_Stringlike* a,
                                const struct LISHP_Object_Stringlike* b)
{
  if (a->hash != b->hash) return false;
  if (a->length != b->length) return false;
  return memcmp(a->data, b->data, a->length) == 0;
}

[[nodiscard]] bool
LISHP_Object_AreEqual(const LISHP_Object* a, const LISHP_Object* b)
{
  assert(a);
  assert(b);

  if (a->tag != b->tag) return false;

  switch (a->tag) {
    case LISHP_OBJECTTAG_NIL : return true;
    case LISHP_OBJECTTAG_CONS: return a == b;
    case LISHP_OBJECTTAG_BOOL:
      return LISHP_Object_AsBool(a)->value == LISHP_Object_AsBool(b)->value;
    case LISHP_OBJECTTAG_CHAR:
      return LISHP_Object_AsChar(a)->value == LISHP_Object_AsChar(b)->value;
    case LISHP_OBJECTTAG_FLOAT:
      return LISHP_Object_AsFloat(a)->value == LISHP_Object_AsFloat(b)->value;
    case LISHP_OBJECTTAG_BIGINT:
      return LISHP_Object_BigintAreEqual(LISHP_Object_AsBigint(a),
                                         LISHP_Object_AsBigint(b));
    case LISHP_OBJECTTAG_RATIO:
      return LISHP_Object_RatioAreEqual(LISHP_Object_AsRatio(a),
                                        LISHP_Object_AsRatio(b));
    case LISHP_OBJECTTAG_SYMBOL:
      return LISHP_Object_StringlikeAreEqual(LISHP_Object_AsSymbol(a),
                                             LISHP_Object_AsSymbol(b));
    case LISHP_OBJECTTAG_KEYWORD:
      return LISHP_Object_StringlikeAreEqual(LISHP_Object_AsKeyword(a),
                                             LISHP_Object_AsKeyword(b));
    case LISHP_OBJECTTAG_STRING:
      return LISHP_Object_StringlikeAreEqual(LISHP_Object_AsString(a),
                                             LISHP_Object_AsString(b));
  }
}

static uint64_t
LISHP_Object_FloatGetHash(const LISHP_Object_Float* f)
{
  // Handling of 0.0 and -0.0. Looks stupid, but is correct
  auto val = f->value == 0.0 ? 0.0 : f->value;

  uint64_t bits;
  static_assert(sizeof(bits) == sizeof(val));

  memcpy(&bits, &val, sizeof(bits));
  return bits;
}

static uint64_t
LISHP_Object_BigintGetHash(const LISHP_Object_Bigint* b)
{
  uint64_t hash = 1469598103934665603ULL;

  for (size_t i = 0; i < b->num_legs; ++i) {
    hash ^= b->legs[i];
    hash *= 1099511628211ULL;
  }

  return hash;
}

static uint64_t
LISHP_Object_RatioGetHash(const LISHP_Object_Ratio* r)
{
  uint64_t h1 = LISHP_Value_GetHash(r->numerator);
  uint64_t h2 = LISHP_Value_GetHash(r->denominator);
  return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
}

[[nodiscard]] uint64_t
LISHP_Object_GetHash(const LISHP_Object* obj)
{
  assert(obj);

  switch (obj->tag) {
    case LISHP_OBJECTTAG_NIL : return 1;
    case LISHP_OBJECTTAG_CONS: return (uint64_t)(uintptr_t)obj;
    case LISHP_OBJECTTAG_BOOL: return LISHP_Object_AsBool(obj)->value ? 2 : 3;
    case LISHP_OBJECTTAG_CHAR: return LISHP_Object_AsChar(obj)->value;
    case LISHP_OBJECTTAG_FLOAT:
      return LISHP_Object_FloatGetHash(LISHP_Object_AsFloat(obj));
    case LISHP_OBJECTTAG_BIGINT:
      return LISHP_Object_BigintGetHash(LISHP_Object_AsBigint(obj));
    case LISHP_OBJECTTAG_RATIO:
      return LISHP_Object_RatioGetHash(LISHP_Object_AsRatio(obj));
    case LISHP_OBJECTTAG_SYMBOL : return LISHP_Object_AsSymbol(obj)->hash;
    case LISHP_OBJECTTAG_KEYWORD: return LISHP_Object_AsKeyword(obj)->hash;
    case LISHP_OBJECTTAG_STRING : return LISHP_Object_AsString(obj)->hash;
  }
}
