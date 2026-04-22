#ifndef LISHP_OBJECT_H
#define LISHP_OBJECT_H

#include "lishp_prelude.h"
#include "lishp_value.h"

#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef enum LISHP_ObjectTag
{
  LISHP_OBJECTTAG_NIL,
  LISHP_OBJECTTAG_CONS,

  LISHP_OBJECTTAG_BOOL,
  LISHP_OBJECTTAG_CHAR,
  LISHP_OBJECTTAG_FLOAT,

  LISHP_OBJECTTAG_BIGINT,
  LISHP_OBJECTTAG_RATIO,

  LISHP_OBJECTTAG_SYMBOL,
  LISHP_OBJECTTAG_KEYWORD,
  LISHP_OBJECTTAG_STRING,
} LISHP_ObjectTag;

typedef struct LISHP_Object_Cons
{
  LISHP_ObjectTag tag;
  LISHP_Value     car;
  LISHP_Value     cdr;
} LISHP_Object_Cons;

typedef struct LISHP_Object_Bool
{
  LISHP_ObjectTag tag;
  bool            value;
} LISHP_Object_Bool;

typedef struct LISHP_Object_Char
{
  LISHP_ObjectTag tag;
  char            value;
} LISHP_Object_Char;

typedef struct LISHP_Object_Float
{
  LISHP_ObjectTag tag;
  double          value;
} LISHP_Object_Float;

typedef struct LISHP_Object_Bigint
{
  LISHP_ObjectTag tag;
  size_t          num_legs;
  uint32_t        legs[];
} LISHP_Object_Bigint;

typedef struct LISHP_Object_Ratio
{
  LISHP_ObjectTag tag;
  LISHP_Value     numerator;
  LISHP_Value     denominator;
} LISHP_Object_Ratio;

struct LISHP_Object_Stringlike
{
  LISHP_ObjectTag tag;
  uint64_t        hash;
  size_t          length;
  char            data[];
};

typedef struct LISHP_Object_Stringlike LISHP_Object_Symbol;
typedef struct LISHP_Object_Stringlike LISHP_Object_Keyword;
typedef struct LISHP_Object_Stringlike LISHP_Object_String;

union LISHP_Object
{
  LISHP_ObjectTag      tag;
  LISHP_Object_Cons    as_cons;
  LISHP_Object_Bool    as_bool;
  LISHP_Object_Char    as_char;
  LISHP_Object_Float   as_float;
  LISHP_Object_Bigint  as_bigint;
  LISHP_Object_Ratio   as_ratio;
  LISHP_Object_Symbol  as_symbol;
  LISHP_Object_Keyword as_keyword;
  LISHP_Object_String  as_string;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

void
LISHP_Object_InitializeCons(LISHP_Object*  obj,
                            LISHP_Value    car,
                            LISHP_Value    cdr,
                            LISHP_Context* ctx);
void
LISHP_Object_InitializeBool(LISHP_Object* obj, bool value, LISHP_Context* ctx);
void
LISHP_Object_InitializeChar(LISHP_Object* obj, char value, LISHP_Context* ctx);
void
LISHP_Object_InitializeFloat(LISHP_Object*  obj,
                             double         value,
                             LISHP_Context* ctx);
void
LISHP_Object_InitializeBigint(LISHP_Object*   obj,
                              size_t          num_legs,
                              const uint32_t* legs,
                              LISHP_Context*  ctx);
void
LISHP_Object_InitializeRatio(LISHP_Object*  obj,
                             LISHP_Value    numerator,
                             LISHP_Value    denominator,
                             LISHP_Context* ctx);
void
LISHP_Object_InitializeSymbol(LISHP_Object*  obj,
                              size_t         len,
                              const char*    data,
                              LISHP_Context* ctx);
void
LISHP_Object_InitializeKeyword(LISHP_Object*  obj,
                               size_t         len,
                               const char*    data,
                               LISHP_Context* ctx);
void
LISHP_Object_InitializeString(LISHP_Object*  obj,
                             size_t         len,
                             const char*    data,
                             LISHP_Context* ctx);

void
LISHP_Object_Finalize(LISHP_Object* obj, LISHP_Context* ctx);

#endif /* LISHP_OBJECT_H */
