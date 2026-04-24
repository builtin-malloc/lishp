#include "lishp/lishp_value.h"
#include "lishp/lishp_object.h"

/*****************************************************************************/
/*                              BASIC OPERATIONS                             */
/*****************************************************************************/

[[nodiscard]] uint64_t
LISHP_Value_GetHash(LISHP_Value val)
{
  if (LISHP_Value_IsBoxed(val))
    return LISHP_Object_GetHash(LISHP_Value_AsBoxed(val));
  return (uint64_t)val;
}

[[nodiscard]] bool
LISHP_Value_AreEqual(LISHP_Value a, LISHP_Value b)
{
  if (a == b) return true;
  if ((a & LISHP_VALUE_TAG_MASK) != (b & LISHP_VALUE_TAG_MASK)) return false;

  if (LISHP_Value_IsBoxed(a))
    return LISHP_Object_AreEqual(LISHP_Value_AsBoxed(a),
                                 LISHP_Value_AsBoxed(b));

  // TODO: Check if all other cases are necessarily false.
  //       I think in all other cases the first line of this function would have
  //       returned true.
  return false;
}
