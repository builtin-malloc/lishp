#include "lishp/lishp_symtab.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_api.h"
#include "lishp/lishp_context.h"
#include "lishp/lishp_diag.h"
#include "lishp/lishp_gc.h"
#include "lishp/lishp_hash.h"
#include "lishp/lishp_object.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_value.h"

#include <assert.h>
#include <string.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Symtab*
LISHP_Symtab_Create(LISHP_Alloc* alloc)
{
  assert(alloc);

  LISHP_Symtab* symtab = LISHP_Alloc_Malloc(alloc, sizeof(*symtab));
  if (!symtab) {
    LISHP_Diag_WriteError(nullptr, "Failed to allocate global symbol table");
    return nullptr;
  }

  symtab->alloc = alloc;

  bool success = true;

  success &= LISHP_HashSet_Initialize(&symtab->symbols, 1024, alloc);
  success &= LISHP_HashSet_Initialize(&symtab->keywords, 512, alloc);

  if (!success) {
    LISHP_Symtab_Destroy(symtab);
    LISHP_Diag_WriteError(nullptr, "Failed to allocate symbol table contents");
    return nullptr;
  }

  return symtab;
}

void
LISHP_Symtab_Destroy(LISHP_Symtab* symtab)
{
  if (!symtab) return;

  LISHP_HashSet_Finalize(&symtab->symbols);
  LISHP_HashSet_Finalize(&symtab->keywords);

  LISHP_Alloc_Free(symtab->alloc, symtab);
}

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

struct LISHP_Symtab_Intern_capture
{
  size_t      len;
  const char* data;
};

static bool
LISHP_Symtab_InternSymbol_pred(LISHP_Value key, void* ctx)
{
  struct LISHP_Symtab_Intern_capture* cap = ctx;

  // TODO: Implement unboned symbols
  auto obj = LISHP_Value_AsBoxed(key);
  auto sym = LISHP_Object_AsSymbol(obj);

  if (sym->length != cap->len) return false;
  return memcmp(sym->data, cap->data, cap->len) == 0;
}

static bool
LISHP_Symtab_InternKeyword_pred(LISHP_Value key, void* ctx)
{
  struct LISHP_Symtab_Intern_capture* cap = ctx;

  // TODO: Implement unboxed keywords
  auto obj = LISHP_Value_AsBoxed(key);
  auto kw  = LISHP_Object_AsKeyword(obj);

  if (kw->length != cap->len) return false;
  return memcmp(kw->data, cap->data, cap->len) == 0;
}

[[nodiscard]] LISHP_Value
LISHP_Symbtab_InternSymbol(LISHP_Symtab*  symtab,
                           size_t         len,
                           const char*    data,
                           LISHP_Context* ctx)
{
  struct LISHP_Symtab_Intern_capture cap = { .len = len, .data = data };

  auto table = &symtab->symbols;
  auto hash  = LISHP_Hash_fnv1a(len, data);
  auto pred  = LISHP_Symtab_InternSymbol_pred;

  auto val = LISHP_HashSet_FindByHash(table, hash, pred, &cap);
  if (val != LISHP_VALUE_INVALID) return val;

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Symbol) + (len + 1) * sizeof(*data);
  auto sym  = LISHP_GC_Malloc(gc, size, ctx);

  assert(sym);
  LISHP_Object_InitializeSymbol(sym, len, data, ctx);

  return LISHP_HashSet_Insert(table, LISHP_Value_MakeBoxed(sym));
}

[[nodiscard]] LISHP_Value
LISHP_Symbtab_InternKeyword(LISHP_Symtab*  symtab,
                            size_t         len,
                            const char*    data,
                            LISHP_Context* ctx)
{
  struct LISHP_Symtab_Intern_capture cap = { .len = len, .data = data };

  auto table = &symtab->keywords;
  auto hash  = LISHP_Hash_fnv1a(len, data);
  auto pred  = LISHP_Symtab_InternKeyword_pred;

  auto val = LISHP_HashSet_FindByHash(table, hash, pred, &cap);
  if (val != LISHP_VALUE_INVALID) return val;

  auto gc   = LISHP_Context_GetGC(ctx);
  auto size = sizeof(LISHP_Object_Keyword) + (len + 1) * sizeof(*data);
  auto kw   = LISHP_GC_Malloc(gc, size, ctx);

  assert(kw);
  LISHP_Object_InitializeKeyword(kw, len, data, ctx);

  return LISHP_HashSet_Insert(table, LISHP_Value_MakeBoxed(kw));
}
