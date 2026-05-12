#ifndef LISHP_OBJECT_H
#define LISHP_OBJECT_H

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_prelude.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef enum LISHP_Object_Tag
{
  LISHP_OBJECT_TAG_NIL,
  LISHP_OBJECT_TAG_CONS,

  LISHP_OBJECT_TAG_SYMBOL,
  LISHP_OBJECT_TAG_KEYWORD,

  LISHP_OBJECT_TAG_FUNCTION,
  LISHP_OBJECT_TAG_MACRO,
} LISHP_Object_Tag;

typedef struct LISHP_Object_Nil
{
  LISHP_Object_Tag tag;
} LISHP_Object_Nil;

typedef struct LISHP_Object_Cons
{
  LISHP_Object_Tag tag;
  LISHP_Value      car;
  LISHP_Value      cdr;
} LISHP_Object_Cons;

typedef struct LISHP_Object_Stringlike LISHP_Object_Symbol;
typedef struct LISHP_Object_Stringlike LISHP_Object_Keyword;

struct LISHP_Object_Stringlike
{
  LISHP_Object_Tag tag;
  uint64_t         hash;
  size_t           length;
  char             data[];
};

typedef struct LISHP_Object_Functionlike LISHP_Object_Function;
typedef struct LISHP_Object_Functionlike LISHP_Object_Macro;

struct LISHP_Object_Functionlike
{
  LISHP_Object_Tag tag;
  LISHP_Value      params;
  LISHP_Value      body;
  LISHP_Value      env;
};

union LISHP_Object
{
  LISHP_Object_Tag      tag;
  LISHP_Object_Nil      as_nil;
  LISHP_Object_Cons     as_cons;
  LISHP_Object_Symbol   as_symbol;
  LISHP_Object_Keyword  as_keyword;
  LISHP_Object_Function as_function;
  LISHP_Object_Macro    as_macro;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Object*
LISHP_Object_InitializeNil(LISHP_Object* obj);

[[nodiscard]] LISHP_Object*
LISHP_Object_InitializeCons(LISHP_Object* obj,
                            LISHP_Value   car,
                            LISHP_Value   cdr);

[[nodiscard]] LISHP_Object*
LISHP_Object_InitializeStringlike(LISHP_Object*                   obj,
                                  struct LISHP_Object_Stringlike* str,
                                  LISHP_Object_Tag                tag,
                                  const char*                     data,
                                  size_t                          len);

[[nodiscard, maybe_unused]] static inline LISHP_Object*
LISHP_Object_Initialize_Symbol(LISHP_Object* obj, const char* data, size_t len)
{
  assert(obj);
  return LISHP_Object_InitializeStringlike(
    obj, &obj->as_symbol, LISHP_OBJECT_TAG_SYMBOL, data, len);
}

[[nodiscard, maybe_unused]] static inline LISHP_Object*
LISHP_Object_Initialize_Keyword(LISHP_Object* obj, const char* data, size_t len)
{
  assert(obj);
  return LISHP_Object_InitializeStringlike(
    obj, &obj->as_keyword, LISHP_OBJECT_TAG_KEYWORD, data, len);
}

[[nodiscard]] LISHP_Object*
LISHP_Object_InitializeFunctionlike(LISHP_Object*                     obj,
                                    struct LISHP_Object_Functionlike* func,
                                    LISHP_Object_Tag                  tag,
                                    LISHP_Value                       params,
                                    LISHP_Value                       body,
                                    LISHP_Value                       env);

[[nodiscard, maybe_unused]] static inline LISHP_Object*
LISHP_Object_Initialize_Function(LISHP_Object* obj,
                                 LISHP_Value   params,
                                 LISHP_Value   body,
                                 LISHP_Value   env)
{
  assert(obj);
  return LISHP_Object_InitializeFunctionlike(
    obj, &obj->as_function, LISHP_OBJECT_TAG_FUNCTION, params, body, env);
}

[[nodiscard, maybe_unused]] static inline LISHP_Object*
LISHP_Object_Initialize_Macro(LISHP_Object* obj,
                              LISHP_Value   params,
                              LISHP_Value   body,
                              LISHP_Value   env)
{
  assert(obj);
  return LISHP_Object_InitializeFunctionlike(
    obj, &obj->as_macro, LISHP_OBJECT_TAG_MACRO, params, body, env);
}

void
LISHP_Object_Finalize(LISHP_Object* obj, LISHP_Allocator* alloc);

/*****************************************************************************/
/*                                  QUERIES                                  */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Object_Tag
LISHP_Object_GetTag(const LISHP_Object* obj)
{
  assert(obj);
  return obj->tag;
}

#endif /* LISHP_OBJECT_H */
