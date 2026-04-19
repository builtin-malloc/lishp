#include "lishp/lishp_diag.h"
#include "lishp/lishp_alloc.h"

#include <assert.h>
#include <stdio.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Diag*
LISHP_Diag_Create(LISHP_Alloc* alloc, const LISHP_Config* config)
{
  assert(alloc);
  assert(config);

  LISHP_Diag* diag = LISHP_Alloc_Malloc(alloc, sizeof(*diag));
  if (!diag) {
    // TODO: Print error
    return nullptr;
  }

  diag->alloc    = alloc;
  diag->user_out = LISHP_DIAGSTREAM_STDERR;
  diag->log_out  = LISHP_DIAGSTREAM_STDERR; // TODO: Write to file

  return diag;
}

void
LISHP_Diag_Destroy(LISHP_Diag* diag)
{
  if (!diag) return;
  LISHP_Alloc_Free(diag->alloc, diag);
}

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

void
LISHP_Diag_Write_VA(LISHP_Diag*     diag,
                    LISHP_DiagLevel level,
                    const char*     file,
                    const char*     func,
                    int             line,
                    const char*     fmt,
                    va_list         args)
{
  assert(file);
  assert(func);
  assert(line > 0);
  assert(fmt);

  static char      emergency_format_buffer[LISHP_DIAG_BUFFER_SIZE];
  static char      emergency_message_buffer[LISHP_DIAG_BUFFER_SIZE];
  LISHP_DiagStream emergency_stream = LISHP_DIAGSTREAM_STDERR;

  auto tag      = LISHP_DiagLevel_ToString(level);
  auto fmt_buf  = diag ? diag->format_buffer : emergency_format_buffer;
  auto msg_buf  = diag ? diag->message_buffer : emergency_message_buffer;
  auto user_out = diag ? &diag->user_out : &emergency_stream;
  auto log_out  = diag ? &diag->log_out : &emergency_stream;

  snprintf(fmt_buf, LISHP_DIAG_BUFFER_SIZE, "[%8.8s] %s", tag, fmt);
  vsnprintf(msg_buf, LISHP_DIAG_BUFFER_SIZE, fmt_buf, args);

  auto should_write_to_user = !LISHP_DiagStream_IsConsole(user_out) ||
                              !LISHP_DiagStream_IsConsole(log_out);

  if (LISHP_DiagStream_CanBeWritten(log_out)) {
    auto handle = LISHP_DiagStream_GetHandle(log_out);
    fprintf(handle, "%s\n", msg_buf);

    if (level >= LISHP_DIAGLEVEL_NOTICE) {
      fprintf(handle, "\tin %s (%s.%d)\n", func, file, line);
    }
  }

  if (LISHP_DiagStream_CanBeWritten(user_out) && should_write_to_user) {
    fprintf(LISHP_DiagStream_GetHandle(user_out), "%s\n", msg_buf);
  }
}
