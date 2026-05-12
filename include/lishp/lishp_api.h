#ifndef LISHP_API_H
#define LISHP_API_H

#include "lishp/lishp_prelude.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

/*****************************************************************************/
/*                                 CONSTANTS                                 */
/*****************************************************************************/

/**
 * @brief Symbol for the OUT-OF-MEMORY error condition
 */
[[maybe_unused]] static const LISHP_Value LISHP_OUT_OF_MEMORY_SYMBOL =
  (LISHP_Value)NULL;
/**
 * @brief nil
 */
[[maybe_unused]] static const LISHP_Value LISHP_NIL = (LISHP_Value)NULL;

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

/*****************************************************************************/
/*                               CONDITION API                               */
/*****************************************************************************/

/**
 * @brief Signals an error
 * @note Either, this transfers flow control to the signal handler for this
 *       condition type or it aborts the entire program.
 */
[[noreturn, maybe_unused]] static inline void
LISHP_Error(LISHP_Context*               ctx,
            [[maybe_unused]] LISHP_Value condition_type,
            [[maybe_unused]] LISHP_Value condition_detail)
{
  assert(ctx);

  abort();
}

#endif /* LISHP_API_H */
