#ifndef LISHP_CONFIG_H
#define LISHP_CONFIG_H

#include "lishp_alloc.h"

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_Config LISHP_Config;

struct LISHP_Config
{
  LISHP_Alloc* alloc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Config*
LISHP_Config_Create(LISHP_Alloc* alloc);
void
LISHP_Config_Destroy(LISHP_Config* config);

#endif /* LISHP_CONFIG_H */
