#ifndef LISHP_STATE_H
#define LISHP_STATE_H

#include "lishp_alloc.h"
#include "lishp_config.h"

#include <assert.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_State LISHP_State;

struct LISHP_State
{
  LISHP_Allocator* alloc;
  LISHP_Config*    config;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_State*
LISHP_State_Create(LISHP_Allocator* alloc);
void
LISHP_State_Destroy(LISHP_State* state);

/*****************************************************************************/
/*                                 ACCESSORS                                 */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Allocator*
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

#endif /* LISHP_STATE_H */
