#ifndef LISHP_SYMTAB_H
#define LISHP_SYMTAB_H

#include "lishp_alloc.h"
#include "lishp_hash.h"
#include "lishp_prelude.h"

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_Symtab LISHP_Symtab;

struct LISHP_Symtab
{
  LISHP_HashSet symbols;
  LISHP_HashSet keywords;
  LISHP_Alloc*  alloc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Symtab*
LISHP_Symtab_Create(LISHP_Alloc* alloc);
void
LISHP_Symtab_Destroy(LISHP_Symtab* symtab);

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

[[nodiscard]] LISHP_Value
LISHP_Symbtab_InternSymbol(LISHP_Symtab*  symtab,
                           size_t         len,
                           const char*    data,
                           LISHP_Context* ctx);
[[nodiscard]] LISHP_Value
LISHP_Symbtab_InternKeyword(LISHP_Symtab*  symtab,
                            size_t         len,
                            const char*    data,
                            LISHP_Context* ctx);

#endif /* LISHP_SYMTAB_H */
