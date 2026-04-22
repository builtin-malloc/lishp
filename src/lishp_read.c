#include "lishp/lishp_read.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_api.h"
#include "lishp/lishp_context.h"
#include "lishp/lishp_diag.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_value.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/*                                 CONSTANTS                                 */
/*****************************************************************************/

static constexpr const char LISHP_READER_CHARCLASS_ATOM_EXTRA_CHARS[] =
  "!#%^&*-_=+'<>:?/|";

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
LISHP_Reader_CharToCharClass(char c)
{
  if (c == '\0') return LISHP_READER_CHARCLASS_EOF;
  if (c == ';') return LISHP_READER_CHARCLASS_LINECOMMENT_START;
  if (isspace(c)) return LISHP_READER_CHARCLASS_WHITESPACE;

  if (isalnum(c) || strchr(LISHP_READER_CHARCLASS_ATOM_EXTRA_CHARS, c))
    return LISHP_READER_CHARCLASS_ATOM;

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

  LISHP_Reader_SkipCommentAndWhitespace(reader, ctx);

  switch (LISHP_Reader_GetCurCharClass(reader)) {
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
    case LISHP_READER_CHARCLASS_ATOM: return LISHP_Reader_ReadAtom(reader, ctx);
    default:
      // TODO: Maybe report errors?
      return LISHP_VALUE_INVALID;
  }
}

static inline bool
LISHP_Reader_CharMatchesSymbol(char cur, size_t idx, const char* symb)
{
  const size_t len = strlen(symb);
  if (idx >= len) return false;
  return symb[idx] == cur;
}

[[nodiscard]] LISHP_Value
LISHP_Reader_ReadAtom(LISHP_Reader* reader, LISHP_Context* ctx)
{
  assert(reader);
  assert(ctx);

  auto cur = LISHP_Reader_GetCurChar(reader);

  auto is_kw        = cur == ':';
  auto is_true_sym  = true;
  auto is_false_sym = true;
  auto is_nil_sym   = true;

  auto     has_sign    = cur == '+' || cur == '-';
  auto     sign        = cur == '-' ? -1 : +1;
  auto     any_digits  = isnumber(cur);
  auto     only_digits = has_sign || isnumber(cur);
  uint32_t int_val     = 0;

  auto   begin = reader->cursor;
  size_t len   = 0;

  while (LISHP_Reader_CharToCharClass(cur) == LISHP_READER_CHARCLASS_ATOM) {
    is_true_sym  &= LISHP_Reader_CharMatchesSymbol(cur, len, "true");
    is_false_sym &= LISHP_Reader_CharMatchesSymbol(cur, len, "false");
    is_nil_sym   &= LISHP_Reader_CharMatchesSymbol(cur, len, "nil");

    any_digits  |= isdigit(cur);
    only_digits &= (!has_sign || len > 0) && isdigit(cur);
    if (isdigit(cur)) {
      auto digit    = cur - '0';
      auto overflow = int_val > ((UINT32_MAX - digit) / 10);

      if (overflow) {
        // TODO: Raise error condition using ctx
        abort();
      }

      int_val *= 10;
      int_val += (cur - '0');
    }

    LISHP_Reader_Advance(reader);
    cur  = LISHP_Reader_GetCurChar(reader);
    len += 1;
  }

  is_true_sym  &= (len == strlen("true"));
  is_false_sym &= (len == strlen("false"));
  is_nil_sym   &= (len == strlen("nil"));

  if (is_kw) return LISHP_CreateKeyword(len, begin, ctx);
  if (is_true_sym) return LISHP_CreateBool(true, ctx);
  if (is_false_sym) return LISHP_CreateBool(false, ctx);
  if (is_nil_sym) return LISHP_CreateNil(ctx);
  if (any_digits && only_digits)
    return LISHP_CreateBigint(sign, 1, &int_val, ctx);

  return LISHP_CreateSymbol(len, begin, ctx);
}

void
LISHP_Reader_SkipComment(LISHP_Reader*                   reader,
                         [[maybe_unused]] LISHP_Context* ctx)
{
  assert(reader);
  assert(ctx);

  auto cls = LISHP_Reader_GetCurCharClass(reader);
  if (cls != LISHP_READER_CHARCLASS_LINECOMMENT_START) return;

  auto c = LISHP_Reader_GetCurChar(reader);
  while (c != '\n' && c != '\0') {
    LISHP_Reader_Advance(reader);
    c = LISHP_Reader_GetCurChar(reader);
  }
}

void
LISHP_Reader_SkipWhitespace(LISHP_Reader*                   reader,
                            [[maybe_unused]] LISHP_Context* ctx)
{
  assert(reader);
  assert(ctx);

  auto cls = LISHP_Reader_GetCurCharClass(reader);
  if (cls != LISHP_READER_CHARCLASS_WHITESPACE) return;

  auto c = LISHP_Reader_GetCurChar(reader);
  while (isspace(c)) {
    LISHP_Reader_Advance(reader);
    c = LISHP_Reader_GetCurChar(reader);
  }
}

void
LISHP_Reader_SkipCommentAndWhitespace(LISHP_Reader*                   reader,
                                      [[maybe_unused]] LISHP_Context* ctx)
{
  assert(reader);
  assert(ctx);

  while (true) {
    switch (LISHP_Reader_GetCurCharClass(reader)) {
      case LISHP_READER_CHARCLASS_LINECOMMENT_START:
        LISHP_Reader_SkipComment(reader, ctx);
        break;
      case LISHP_READER_CHARCLASS_WHITESPACE:
        LISHP_Reader_SkipWhitespace(reader, ctx);
        break;
      default: return;
    }
  }
}
