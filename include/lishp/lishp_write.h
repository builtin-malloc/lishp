#ifndef LISHP_WRITE_H
#define LISHP_WRITE_H

#include "lishp_alloc.h"
#include "lishp_prelude.h"
#include "lishp_value.h"

#include <stddef.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_Writer LISHP_Writer;

typedef enum LISHP_WriteMode
{
  LISHP_WRITEMODE_RAW,
  LISHP_WRITEMODE_READABLE,
  LISHP_WRITEMODE_PRETTY,
} LISHP_WriteMode;

struct LISHP_Writer
{
  LISHP_Alloc*    alloc;
  LISHP_WriteMode mode;
  size_t          buffer_size;
  size_t          buffer_capacity;
  char*           buffer;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Writer*
LISHP_Writer_Create(LISHP_Alloc* alloc);
void
LISHP_Writer_Destroy(LISHP_Writer* writer);

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] const char*
LISHP_Writer_Write(LISHP_Writer*   writer,
                   LISHP_Value     value,
                   LISHP_WriteMode mode,
                   LISHP_Context*  ctx);

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

void
LISHP_Writer_Reset(LISHP_Writer* writer, LISHP_WriteMode mode);

[[nodiscard, maybe_unused]] static inline bool
LISHP_Writer_BufferHasSpace(const LISHP_Writer* writer, size_t num_chars)
{
  assert(writer);
  if (!writer->buffer) return false;
  if (num_chars + 1 > writer->buffer_capacity) return false;
  return writer->buffer_size < writer->buffer_capacity - num_chars - 1;
}

[[nodiscard]] bool
LISHP_Writer_AllocateMoreBufferSpace(LISHP_Writer* writer);

/*****************************************************************************/
/*                           WRITER IMPLEMENTATION                           */
/*****************************************************************************/

void
LISHP_Writer_AppendValue(LISHP_Writer* writer, LISHP_Value val);

void
LISHP_Writer_AppendInvalidValue(LISHP_Writer* writer);
void
LISHP_Writer_AppendObject(LISHP_Writer* writer, const LISHP_Object* obj);

void
LISHP_Writer_AppendNilObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendConsObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendBoolObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendCharObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendFloatObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendBigintObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendRatioObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendSymbolObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendKeywordObject(LISHP_Writer* writer, const LISHP_Object* obj);
void
LISHP_Writer_AppendStringObject(LISHP_Writer* writer, const LISHP_Object* obj);

void
LISHP_Writer_AppendString(LISHP_Writer* writer, const char* str);
void
LISHP_Writer_AppendStringWithLength(LISHP_Writer* writer,
                                    size_t        len,
                                    const char*   str);
void
LISHP_Writer_AppendAddress(LISHP_Writer* writer, const void* addr);

#endif /* LISHP_WRITE_H */
