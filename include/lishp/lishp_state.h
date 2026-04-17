#ifndef LISHP_STATE_H
#define LISHP_STATE_H

#include "lishp_alloc.h"
#include "lishp_config.h"
#include "lishp_diag.h"
#include "lishp_gc.h"
#include "lishp_read.h"
#include "lishp_symtab.h"
#include "lishp_vm.h"

#include <assert.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_State LISHP_State;

struct LISHP_State
{
  LISHP_Alloc*  alloc;
  LISHP_Config* config;
  LISHP_Diag*   diag;
  LISHP_GC*     gc;
  LISHP_VM*     vm;
  LISHP_Symtab* symtab;
  LISHP_Reader* reader;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_State*
LISHP_State_Create(LISHP_Alloc* alloc);
void
LISHP_State_Destroy(LISHP_State* state);

/*****************************************************************************/
/*                                 ACCESSORS                                 */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Alloc*
LISHP_State_GetAlloc(LISHP_State* state)
{
  assert(state);
  return state->alloc;
}

[[nodiscard, maybe_unused]] static inline LISHP_Config*
LISHP_State_GetConfig(LISHP_State* state)
{
  assert(state);
  return state->config;
}

[[nodiscard, maybe_unused]] static inline LISHP_Diag*
LISHP_State_GetDiag(LISHP_State* state)
{
  assert(state);
  return state->diag;
}

[[nodiscard, maybe_unused]] static inline LISHP_GC*
LISHP_State_GetGC(LISHP_State* state)
{
  assert(state);
  return state->gc;
}

[[nodiscard, maybe_unused]] static inline LISHP_VM*
LISHP_State_GetVM(LISHP_State* state)
{
  assert(state);
  return state->vm;
}

[[nodiscard, maybe_unused]] static inline LISHP_Symtab*
LISHP_State_GetSymtab(LISHP_State* state)
{
  assert(state);
  return state->symtab;
}

[[nodiscard, maybe_unused]] static inline LISHP_Reader*
LISHP_State_GetReader(LISHP_State* state)
{
  assert(state);
  return state->reader;
}

#endif /* LISHP_STATE_H */
