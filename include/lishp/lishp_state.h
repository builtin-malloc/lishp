#ifndef LISHP_STATE_H
#define LISHP_STATE_H

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_State LISHP_State;

struct LISHP_State
{
  int dummy;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_State*
LISHP_State_Create();
void
LISHP_State_Destroy(LISHP_State* state);

#endif /* LISHP_STATE_H */
