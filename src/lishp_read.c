#include "lishp/lishp_read.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_value.h"

#include <assert.h>
#include <ctype.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Reader*
LISHP_Reader_Create(LISHP_Alloc* alloc, LISHP_GC* gc)
{
  assert(alloc);

  LISHP_Reader* reader = LISHP_Alloc_Malloc(alloc, sizeof(*reader));
  if (!reader) {
    // TODO: Print Error
    return nullptr;
  }

  reader->alloc  = alloc;
  reader->gc     = gc;
  reader->cursor = nullptr;

  return reader;
}

void
LISHP_Reader_Destroy(LISHP_Reader* reader)
{
  if (!reader) return;
  LISHP_Alloc_Free(reader->alloc, reader);
}

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] LISHP_Value
LISHP_Reader_Read(LISHP_Reader* reader, const char* src, LISHP_Context* ctx)
{
  if (!reader) return LISHP_VALUE_INVALID;
  if (!src) return LISHP_VALUE_INVALID;
  if (!ctx) return LISHP_VALUE_INVALID;

  LISHP_Reader_Reset(reader, src);
  return LISHP_Reader_ReadValue(reader, ctx);
}

/*****************************************************************************/
/*                                  QUERIES                                  */
/*****************************************************************************/

[[nodiscard]] LISHP_Reader_CharClass
LISHP_Reader_GetCurCharClass(const LISHP_Reader* reader)
{
  assert(reader);
  auto c = LISHP_Reader_GetCurChar(reader);

  if (c == '\0') return LISHP_READER_CHARCLASS_EOF;
  if (c == ';') return LISHP_READER_CHARCLASS_LINECOMMENT_START;
  if (isspace(c)) return LISHP_READER_CHARCLASS_WHITESPACE;

  // TODO: Report error here?
  return LISHP_READER_CHARCLASS_ERROR;
}

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

void
LISHP_Reader_Reset(LISHP_Reader* reader, const char* src)
{
  assert(reader);
  assert(src);

  reader->cursor = src;
}

/*****************************************************************************/
/*                           READER IMPLEMENTATION                           */
/*****************************************************************************/

[[nodiscard]] LISHP_Value
LISHP_Reader_ReadValue(LISHP_Reader* reader, LISHP_Context* ctx)
{
  assert(reader);
  assert(ctx);

  LISHP_Reader_SkipCommentAndWhitespace(reader);

  switch (LISHP_Reader_GetCurChar(reader)) {
    case LISHP_READER_CHARCLASS_ERROR:
      // TODO: Maybe report errors?
      return LISHP_VALUE_INVALID;
    case LISHP_READER_CHARCLASS_EOF:
      // TODO: Maybe report errors?
      return LISHP_VALUE_INVALID;
    case LISHP_READER_CHARCLASS_WHITESPACE:
      // TODO: Maybe report errors?
      return LISHP_VALUE_INVALID;
    case LISHP_READER_CHARCLASS_LINECOMMENT_START:
      // TODO: Maybe report errors?
      return LISHP_VALUE_INVALID;
    default:
      // TODO: Maybe report errors?
      return LISHP_VALUE_INVALID;
  }
}

void
LISHP_Reader_SkipComment(LISHP_Reader* reader)
{
  assert(reader);

  auto cls = LISHP_Reader_GetCurCharClass(reader);
  if (cls != LISHP_READER_CHARCLASS_LINECOMMENT_START) return;

  auto c = LISHP_Reader_GetCurChar(reader);
  while (c != '\n' && c != '\0') {
    LISHP_Reader_Advance(reader);
    c = LISHP_Reader_GetCurChar(reader);
  }
}

void
LISHP_Reader_SkipWhitespace(LISHP_Reader* reader)
{
  assert(reader);

  auto cls = LISHP_Reader_GetCurCharClass(reader);
  if (cls != LISHP_READER_CHARCLASS_WHITESPACE) return;

  auto c = LISHP_Reader_GetCurChar(reader);
  while (isspace(c)) {
    LISHP_Reader_Advance(reader);
    c = LISHP_Reader_GetCurChar(reader);
  }
}

void
LISHP_Reader_SkipCommentAndWhitespace(LISHP_Reader* reader)
{
  assert(reader);

  while (true) {
    switch (LISHP_Reader_GetCurCharClass(reader)) {
      case LISHP_READER_CHARCLASS_LINECOMMENT_START:
        LISHP_Reader_SkipComment(reader);
        break;
      case LISHP_READER_CHARCLASS_WHITESPACE:
        LISHP_Reader_SkipWhitespace(reader);
        break;
      default: return;
    }
  }
}
