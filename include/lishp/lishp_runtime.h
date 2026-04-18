#ifndef LISHP_RUNTIME_H
#define LISHP_RUNTIME_H

#include "lishp_alloc.h"
#include "lishp_config.h"
#include "lishp_gc.h"
#include "lishp_prelude.h"
#include "lishp_symtab.h"

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

struct LISHP_Runtime
{
  LISHP_Alloc*  alloc;
  LISHP_Config* config;
  LISHP_GC*     gc;
  LISHP_Symtab* symtab;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Runtime*
LISHP_Runtime_Create(LISHP_Alloc* alloc);
void
LISHP_Runtime_Destroy(LISHP_Runtime* runtime);

/*****************************************************************************/
/*                                 ACCESSORS                                 */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Alloc*
LISHP_Runtime_GetAlloc(LISHP_Runtime* runtime)
{
  assert(runtime);
  return runtime->alloc;
}

[[nodiscard, maybe_unused]] static inline LISHP_Config*
LISHP_Runtime_GetConfig(LISHP_Runtime* runtime)
{
  assert(runtime);
  return runtime->config;
}

[[nodiscard, maybe_unused]] static inline LISHP_GC*
LISHP_Runtime_GetGC(LISHP_Runtime* runtime)
{
  assert(runtime);
  return runtime->gc;
}

[[nodiscard, maybe_unused]] static inline LISHP_Symtab*
LISHP_Runtime_GetSymtab(LISHP_Runtime* runtime)
{
  assert(runtime);
  return runtime->symtab;
}

#endif /* LISHP_RUNTIME_H */
