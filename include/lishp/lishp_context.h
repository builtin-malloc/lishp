#ifndef LISHP_CONTEXT_H
#define LISHP_CONTEXT_H

#include "lishp_diag.h"
#include "lishp_prelude.h"
#include "lishp_read.h"
#include "lishp_runtime.h"
#include "lishp_vm.h"
#include "lishp_write.h"

#include <assert.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

struct LISHP_Context
{
  LISHP_Runtime* runtime;
  LISHP_Diag*    diag;
  LISHP_VM*      vm;
  LISHP_Reader*  reader;
  LISHP_Writer*  writer;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Context*
LISHP_Context_Create(LISHP_Runtime* runtime);
void
LISHP_Context_Destroy(LISHP_Context* ctx);

/*****************************************************************************/
/*                                 ACCESSORS                                 */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Alloc*
LISHP_Context_GetAlloc(LISHP_Context* ctx)
{
  assert(ctx);
  return LISHP_Runtime_GetAlloc(ctx->runtime);
}

[[nodiscard, maybe_unused]] static inline LISHP_Config*
LISHP_Context_GetConfig(LISHP_Context* ctx)
{
  assert(ctx);
  return LISHP_Runtime_GetConfig(ctx->runtime);
}

[[nodiscard, maybe_unused]] static inline LISHP_GC*
LISHP_Context_GetGC(LISHP_Context* ctx)
{
  assert(ctx);
  return LISHP_Runtime_GetGC(ctx->runtime);
}

[[nodiscard, maybe_unused]] static inline LISHP_Symtab*
LISHP_Context_GetSymtab(LISHP_Context* ctx)
{
  assert(ctx);
  return LISHP_Runtime_GetSymtab(ctx->runtime);
}

[[nodiscard, maybe_unused]] static inline LISHP_Runtime*
LISHP_Context_GetRuntime(LISHP_Context* ctx)
{
  assert(ctx);
  return ctx->runtime;
}

[[nodiscard, maybe_unused]] static inline LISHP_Diag*
LISHP_Context_GetDiag(LISHP_Context* ctx)
{
  assert(ctx);
  return ctx->diag;
}

[[nodiscard, maybe_unused]] static inline LISHP_VM*
LISHP_Context_GetVM(LISHP_Context* ctx)
{
  assert(ctx);
  return ctx->vm;
}

[[nodiscard, maybe_unused]] static inline LISHP_Reader*
LISHP_Context_GetReader(LISHP_Context* ctx)
{
  assert(ctx);
  return ctx->reader;
}

[[nodiscard, maybe_unused]] static inline LISHP_Writer*
LISHP_Context_GetWriter(LISHP_Context* ctx)
{
  assert(ctx);
  return ctx->writer;
}

#endif /* LISHP_CONTEXT_H */
