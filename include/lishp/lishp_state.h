#ifndef LISHP_STATE_H
#define LISHP_STATE_H

#include "lishp_alloc.h"
#include "lishp_config.h"
#include "lishp_diag.h"

#include <assert.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_State LISHP_State;

struct LISHP_State
{
  LISHP_Alloc* alloc;
  LISHP_Config*    config;
  LISHP_Diag*      diag;
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

#endif /* LISHP_STATE_H */
