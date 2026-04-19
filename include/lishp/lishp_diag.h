#ifndef LISHP_DIAG_H
#define LISHP_DIAG_H

#include "lishp_alloc.h"
#include "lishp_config.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

/*****************************************************************************/
/*                                   MACROS                                  */
/*****************************************************************************/

#define LISHP_DIAGSTREAM_NULL                                                  \
  ((LISHP_DiagStream){ .handle = nullptr, .is_console = false })
#define LISHP_DIAGSTREAM_STDOUT                                                \
  ((LISHP_DiagStream){ .handle = stdout, .is_console = true })
#define LISHP_DIAGSTREAM_STDERR                                                \
  ((LISHP_DiagStream){ .handle = stderr, .is_console = true })

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

constexpr size_t LISHP_DIAG_BUFFER_SIZE = 4 * 1024;

typedef struct LISHP_Diag       LISHP_Diag;
typedef struct LISHP_DiagStream LISHP_DiagStream;

struct LISHP_DiagStream
{
  FILE* handle;
  bool  is_console;
};

struct LISHP_Diag
{
  LISHP_Alloc*     alloc;
  LISHP_DiagStream user_out;
  LISHP_DiagStream log_out;
  char             format_buffer[LISHP_DIAG_BUFFER_SIZE];
  char             message_buffer[LISHP_DIAG_BUFFER_SIZE];
};

typedef enum LISHP_DiagLevel
{
  LISHP_DIAGLEVEL_TRACE,
  LISHP_DIAGLEVEL_VERBOSE,
  LISHP_DIAGLEVEL_DEBUG,
  LISHP_DIAGLEVEL_INFO,
  LISHP_DIAGLEVEL_NOTICE,
  LISHP_DIAGLEVEL_WARN,
  LISHP_DIAGLEVEL_ERROR,
  LISHP_DIAGLEVEL_FATAL,
} LISHP_DiagLevel;

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Diag*
LISHP_Diag_Create(LISHP_Alloc* alloc, const LISHP_Config* config);
void
LISHP_Diag_Destroy(LISHP_Diag* diag);

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

#define LISHP_DIAG_WRITE__TEMPLATE(Diag, Level, Fmt, ...)                      \
  LISHP_Diag_Write(                                                            \
    Diag, Level, __FILE__, __func__, __LINE__, Fmt __VA_OPT__(, ) __VA_ARGS__)

#define LISHP_Diag_WriteTrace(Diag)                                            \
  LISHP_DIAG_WRITE__TEMPLATE(Diag, LISHP_DIAGLEVEL_TRACE, "%s", __func__)
#define LISHP_Diag_Verbose(Diag, Fmt, ...)                                     \
  LISHP_DIAG_WRITE__TEMPLATE(                                                  \
    Diag, LISHP_DIAGLEVEL_VERBOSE, Fmt __VA_OPT__(, ) __VA_ARGS__)
#define LISHP_Diag_WriteDebug(Diag, Fmt, ...)                                  \
  LISHP_DIAG_WRITE__TEMPLATE(                                                  \
    Diag, LISHP_DIAGLEVEL_DEBUG, Fmt __VA_OPT__(, ) __VA_ARGS__)
#define LISHP_Diag_WriteInfo(Diag, Fmt, ...)                                   \
  LISHP_DIAG_WRITE__TEMPLATE(                                                  \
    Diag, LISHP_DIAGLEVEL_INFO, Fmt __VA_OPT__(, ) __VA_ARGS__)
#define LISHP_Diag_WriteNotice(Diag, Fmt, ...)                                 \
  LISHP_DIAG_WRITE__TEMPLATE(                                                  \
    Diag, LISHP_DIAGLEVEL_NOTICE, Fmt __VA_OPT__(, ) __VA_ARGS__)
#define LISHP_Diag_WriteWarn(Diag, Fmt, ...)                                   \
  LISHP_DIAG_WRITE__TEMPLATE(                                                  \
    Diag, LISHP_DIAGLEVEL_WARN, Fmt __VA_OPT__(, ) __VA_ARGS__)
#define LISHP_Diag_WriteError(Diag, Fmt, ...)                                  \
  LISHP_DIAG_WRITE__TEMPLATE(                                                  \
    Diag, LISHP_DIAGLEVEL_ERROR, Fmt __VA_OPT__(, ) __VA_ARGS__)
#define LISHP_Diag_WriteFatal(Diag, Fmt, ...)                                  \
  LISHP_DIAG_WRITE__TEMPLATE(                                                  \
    Diag, LISHP_DIAGLEVEL_FATAL, Fmt __VA_OPT__(, ) __VA_ARGS__)

void
LISHP_Diag_Write_VA(LISHP_Diag*     diag,
                    LISHP_DiagLevel level,
                    const char*     file,
                    const char*     func,
                    int             line,
                    const char*     fmt,
                    va_list         args);

[[maybe_unused]] static inline void
LISHP_Diag_Write(LISHP_Diag*     diag,
                 LISHP_DiagLevel level,
                 const char*     file,
                 const char*     func,
                 int             line,
                 const char*     fmt,
                 ...)
{
  assert(file);
  assert(func);
  assert(line > 0);
  assert(fmt);

  va_list args, forward;
  va_start(args, fmt);
  va_copy(forward, args);

  LISHP_Diag_Write_VA(diag, level, file, func, line, fmt, forward);

  va_end(forward);
  va_end(args);
}

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline const char*
LISHP_DiagLevel_ToString(LISHP_DiagLevel lvl)
{
  switch (lvl) {
    case LISHP_DIAGLEVEL_TRACE  : return "TRACE";
    case LISHP_DIAGLEVEL_VERBOSE: return "VERBOSE";
    case LISHP_DIAGLEVEL_DEBUG  : return "DEBUG";
    case LISHP_DIAGLEVEL_INFO   : return "INFO";
    case LISHP_DIAGLEVEL_NOTICE : return "NOTICE";
    case LISHP_DIAGLEVEL_WARN   : return "WARN";
    case LISHP_DIAGLEVEL_ERROR  : return "ERROR";
    case LISHP_DIAGLEVEL_FATAL  : return "FATAL";
  }
}

[[nodiscard, maybe_unused]] static inline bool
LISHP_DiagStream_CanBeWritten(const LISHP_DiagStream* stream)
{
  assert(stream);
  return stream->handle != nullptr;
}

[[nodiscard, maybe_unused]] static inline bool
LISHP_DiagStream_IsConsole(const LISHP_DiagStream* stream)
{
  assert(stream);
  return stream->is_console;
}

[[nodiscard, maybe_unused]] static inline FILE*
LISHP_DiagStream_GetHandle(const LISHP_DiagStream* stream)
{
  assert(stream);
  return stream->handle;
}

#endif /* LISHP_DIAG_H */
