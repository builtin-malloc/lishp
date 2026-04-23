#include "lishp/lishp_write.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_object.h"
#include "lishp/lishp_value.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Writer*
LISHP_Writer_Create(LISHP_Alloc* alloc)
{
  assert(alloc);

  LISHP_Writer* writer = LISHP_Alloc_Malloc(alloc, sizeof(*writer));
  if (!writer) {
    // TODO: Print Error
    return nullptr;
  }

  writer->alloc           = alloc;
  writer->mode            = LISHP_WRITEMODE_RAW;
  writer->buffer_size     = 0;
  writer->buffer_capacity = 2048;
  writer->buffer          = LISHP_Alloc_Malloc(alloc, writer->buffer_capacity);

  if (!writer->buffer) {
    // TODO: Print Error
    LISHP_Writer_Destroy(writer);
    return nullptr;
  }

  return writer;
}

void
LISHP_Writer_Destroy(LISHP_Writer* writer)
{
  if (!writer) return;

  LISHP_Alloc_Free(writer->alloc, writer->buffer);
  LISHP_Alloc_Free(writer->alloc, writer);
}

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] const char*
LISHP_Writer_Write(LISHP_Writer*   writer,
                   LISHP_Value     value,
                   LISHP_WriteMode mode,
                   LISHP_Context*  ctx)
{
  assert(writer);
  assert(ctx);

  LISHP_Writer_Reset(writer, mode);
  LISHP_Writer_AppendValue(writer, value);

  return writer->buffer;
}

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

void
LISHP_Writer_Reset(LISHP_Writer* writer, LISHP_WriteMode mode)
{
  assert(writer);
  writer->mode        = mode;
  writer->buffer_size = 0;
  if (writer->buffer) memset(writer->buffer, 0, writer->buffer_capacity);
}

[[nodiscard]] bool
LISHP_Writer_AllocateMoreBufferSpace(LISHP_Writer* writer)
{
  assert(writer);

  auto alloc   = writer->alloc;
  auto old_cap = writer->buffer_capacity;
  auto old_buf = writer->buffer;
  auto new_cap = old_cap > 0 ? 2 * old_cap : 512;
  auto new_buf = LISHP_Alloc_Realloc(alloc, old_buf, new_cap);

  if (!new_buf) {
    // TODO: Print error
    return false;
  }

  writer->buffer          = new_buf;
  writer->buffer_capacity = new_cap;

  return true;
}

/*****************************************************************************/
/*                           WRITER IMPLEMENTATION                           */
/*****************************************************************************/

void
LISHP_Writer_AppendValue(LISHP_Writer* writer, LISHP_Value val)
{
  assert(writer);

  if (val == LISHP_VALUE_INVALID) {
    LISHP_Writer_AppendInvalidValue(writer);
    return;
  }

  if (LISHP_Value_IsBoxed(val)) {
    LISHP_Writer_AppendObject(writer, LISHP_Value_AsBoxed(val));
    return;
  }
}

void
LISHP_Writer_AppendInvalidValue(LISHP_Writer* writer)
{
  assert(writer);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<INVALID: 0x00>");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, "<INVALID>");
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, "<INVALID>");
      break;
  }
}

void
LISHP_Writer_AppendObject(LISHP_Writer* writer, const LISHP_Object* object)
{
  assert(writer);
  assert(object);

  switch (object->tag) {
    case LISHP_OBJECTTAG_NIL:
      LISHP_Writer_AppendNilObject(writer, object);
      break;
    case LISHP_OBJECTTAG_CONS:
      LISHP_Writer_AppendConsObject(writer, object);
      break;
    case LISHP_OBJECTTAG_BOOL:
      LISHP_Writer_AppendBoolObject(writer, object);
      break;
    case LISHP_OBJECTTAG_CHAR:
      LISHP_Writer_AppendCharObject(writer, object);
      break;
    case LISHP_OBJECTTAG_FLOAT:
      LISHP_Writer_AppendFloatObject(writer, object);
      break;
    case LISHP_OBJECTTAG_BIGINT:
      LISHP_Writer_AppendBigintObject(writer, object);
      break;
    case LISHP_OBJECTTAG_RATIO:
      LISHP_Writer_AppendRatioObject(writer, object);
      break;
    case LISHP_OBJECTTAG_SYMBOL:
      LISHP_Writer_AppendSymbolObject(writer, object);
      break;
    case LISHP_OBJECTTAG_KEYWORD:
      LISHP_Writer_AppendKeywordObject(writer, object);
      break;
    case LISHP_OBJECTTAG_STRING:
      LISHP_Writer_AppendStringObject(writer, object);
      break;
  }
}

void
LISHP_Writer_AppendNilObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<NIL: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, ">");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, "nil");
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, "nil");
      break;
  }
}

void
LISHP_Writer_AppendConsObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<CONS: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, ">");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
  }
}

void
LISHP_Writer_AppendBoolObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  auto as_bool = LISHP_Object_AsBool(obj);
  auto as_str  = as_bool->value ? "true" : "false";
  auto as_int  = as_bool->value ? "0x01" : "0x00";

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<BOOL: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, " (");
      LISHP_Writer_AppendString(writer, as_int);
      LISHP_Writer_AppendString(writer, ")>");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, as_str);
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, as_str);
      break;
  }
}

void
LISHP_Writer_AppendCharObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<CHAR: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, ">");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
  }
}

void
LISHP_Writer_AppendFloatObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<FLOAT: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, ">");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
  }
}

void
LISHP_Writer_AppendBigintObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<BIGINT: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, ">");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
  }
}

void
LISHP_Writer_AppendRatioObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<RATIO: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, ">");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
  }
}

void
LISHP_Writer_AppendSymbolObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  auto as_sym = LISHP_Object_AsSymbol(obj);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<SYMBOL: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, " (");
      LISHP_Writer_AppendStringWithLength(writer, as_sym->length, as_sym->data);
      LISHP_Writer_AppendString(writer, ")>");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendStringWithLength(writer, as_sym->length, as_sym->data);
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendStringWithLength(writer, as_sym->length, as_sym->data);
      break;
  }
}

void
LISHP_Writer_AppendKeywordObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  auto as_kw = LISHP_Object_AsKeyword(obj);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<KEYWORD: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, " (:");
      LISHP_Writer_AppendStringWithLength(writer, as_kw->length, as_kw->data);
      LISHP_Writer_AppendString(writer, ")>");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, ":");
      LISHP_Writer_AppendStringWithLength(writer, as_kw->length, as_kw->data);
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, ":");
      LISHP_Writer_AppendStringWithLength(writer, as_kw->length, as_kw->data);
      break;
  }
}

void
LISHP_Writer_AppendStringObject(LISHP_Writer* writer, const LISHP_Object* obj)
{
  assert(writer);
  assert(obj);

  switch (writer->mode) {
    case LISHP_WRITEMODE_RAW:
      LISHP_Writer_AppendString(writer, "<STRING: ");
      LISHP_Writer_AppendAddress(writer, obj);
      LISHP_Writer_AppendString(writer, ">");
      break;
    case LISHP_WRITEMODE_READABLE:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
    case LISHP_WRITEMODE_PRETTY:
      LISHP_Writer_AppendString(writer, "TODO");
      break;
  }
}

void
LISHP_Writer_AppendString(LISHP_Writer* writer, const char* str)
{
  assert(writer);
  assert(str);
  LISHP_Writer_AppendStringWithLength(writer, strlen(str), str);
}

void
LISHP_Writer_AppendStringWithLength(LISHP_Writer* writer,
                                    size_t        len,
                                    const char*   str)
{
  assert(writer);
  assert(str);

  while (!LISHP_Writer_BufferHasSpace(writer, len)) {
    if (!LISHP_Writer_AllocateMoreBufferSpace(writer)) {
      // TODO: Raise error condition
      return;
    }
  }

  assert(writer->buffer_size + len < writer->buffer_capacity - 1);

  char* cursor = writer->buffer + writer->buffer_size;
  memcpy(cursor, str, len);
  cursor[len] = '\0';

  writer->buffer_size += len;
}

void
LISHP_Writer_AppendAddress(LISHP_Writer* writer, const void* addr)
{
  assert(writer);

  // `2 * sizeof(void*)` since each byte in an address needs 2 chars in hex.
  // `2 + ...`           since we prefix the address with 0x
  // `... + 1`           since we need the NUL byte
  constexpr size_t BUF_SIZE = 2 + 2 * sizeof(void*) + 1;
  constexpr size_t BUF_LEN  = BUF_SIZE - 1;

  char buf[BUF_SIZE];
  auto len = snprintf(buf, sizeof(buf), "%p", addr);

  assert(len > 0 && len <= (int)BUF_LEN);

  LISHP_Writer_AppendStringWithLength(writer, len, buf);
}
