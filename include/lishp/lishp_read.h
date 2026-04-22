#ifndef LISHP_READ_H
#define LISHP_READ_H

#include "lishp_alloc.h"
#include "lishp_gc.h"
#include "lishp_prelude.h"
#include "lishp_value.h"

#include <assert.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_Reader LISHP_Reader;

struct LISHP_Reader
{
  LISHP_Alloc* alloc;
  LISHP_GC*    gc;
  const char*  cursor;
};

typedef enum LISHP_Reader_CharClass
{
  LISHP_READER_CHARCLASS_ERROR,
  LISHP_READER_CHARCLASS_EOF,
  LISHP_READER_CHARCLASS_WHITESPACE,
  LISHP_READER_CHARCLASS_LINECOMMENT_START,
  LISHP_READER_CHARCLASS_ATOM,
} LISHP_Reader_CharClass;

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Reader*
LISHP_Reader_Create(LISHP_Alloc* alloc, LISHP_GC* gc);
void
LISHP_Reader_Destroy(LISHP_Reader* reader);

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] LISHP_Value
LISHP_Reader_Read(LISHP_Reader* reader, const char* src, LISHP_Context* ctx);

/*****************************************************************************/
/*                                  QUERIES                                  */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline bool
LISHP_Reader_IsDone(const LISHP_Reader* reader)
{
  assert(reader);
  return !reader->cursor || !*reader->cursor;
}

[[nodiscard, maybe_unused]] static inline char
LISHP_Reader_GetCurChar(const LISHP_Reader* reader)
{
  assert(reader);
  if (!reader->cursor) return '\0';
  return *reader->cursor;
}

[[maybe_unused]] static inline void
LISHP_Reader_Advance(LISHP_Reader* reader)
{
  assert(reader);
  if (LISHP_Reader_IsDone(reader)) return;
  reader->cursor += 1;
}

[[nodiscard]] LISHP_Reader_CharClass
LISHP_Reader_GetCurCharClass(const LISHP_Reader* reader);

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

void
LISHP_Reader_Reset(LISHP_Reader* reader, const char* src);

/*****************************************************************************/
/*                           READER IMPLEMENTATION                           */
/*****************************************************************************/

[[nodiscard]] LISHP_Value
LISHP_Reader_ReadValue(LISHP_Reader* reader, LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_Reader_ReadAtom(LISHP_Reader* reader, LISHP_Context* ctx);

void
LISHP_Reader_SkipComment(LISHP_Reader* reader, LISHP_Context* ctx);
void
LISHP_Reader_SkipWhitespace(LISHP_Reader* reader, LISHP_Context* ctx);
void
LISHP_Reader_SkipCommentAndWhitespace(LISHP_Reader* reader, LISHP_Context* ctx);

#endif /* LISHP_READ_H */
