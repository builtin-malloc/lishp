#ifndef LISHP_API_H
#define LISHP_API_H

#include "lishp/lishp_prelude.h"

#include <stddef.h>

/*****************************************************************************/
/*                                  REPL API                                 */
/*****************************************************************************/

/**
 * @brief Reads ONE form from the src
 * @note Any form after the first one is ignored
 */
[[nodiscard, maybe_unused]] static inline LISHP_Value
LISHP_Read(LISHP_Context* ctx, const char* src)
{
  if (!ctx) return (LISHP_Value)NULL;
  if (!src) return (LISHP_Value)NULL;
  return (LISHP_Value)NULL;
}

/**
 * @brief Expands macros in the form
 */
[[nodiscard, maybe_unused]] static inline LISHP_Value
LISHP_Expand(LISHP_Context* ctx, LISHP_Value form)
{
  if (!ctx) return (LISHP_Value)NULL;
  return form;
}

/**
 * @brief Evaluates the form
 */
[[nodiscard, maybe_unused]] static inline LISHP_Value
LISHP_Evaluate(LISHP_Context* ctx, LISHP_Value form)
{
  if (!ctx) return (LISHP_Value)NULL;
  return form;
}

/**
 * @returns A string representation of the form
 * @note The returned string is owned by the context, not the caller
 * @note The returned string is only valid until the next print
 */
[[nodiscard, maybe_unused]] static inline const char*
LISHP_Print(LISHP_Context* ctx, [[maybe_unused]] LISHP_Value form)
{
  if (!ctx) return NULL;
  return "repr";
}

#endif /* LISHP_API_H */
