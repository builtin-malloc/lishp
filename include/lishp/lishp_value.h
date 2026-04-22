#ifndef LISHP_VALUE_H
#define LISHP_VALUE_H

#include "lishp_prelude.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

/**
 * @brief Boxed and unboxed objects
 *
 * Values are basically pointers that can alternatively be interpreted
 * as immediate (unboxed) values.  In order to differentiate between
 * boxed objects and unboxed values, we need some tag in the
 * value. For this, we use the fact that pointers (on both 32 and 64
 * bit) are usually at least 8-aligned. To differentiate between boxed
 * values (aka pointers) and unboxed values, we just inspect the least
 * significant bit of the value. We know that it must be 0 if it is a
 * pointer, otherwise the pointer would not even be 2 aligned.
 *
 * This gives us the first level of tags:
 *
 *    [63 bits][0] => Pointer (64 bit system)
 *    [63 bits][1] => Unboxed value (64 bit system)
 *    [31 bits][0] => Pointer (32 bit system)
 *    [31 bits][1] => Unboxed value (32 bit system)
 *
 * This leaves us with 63 (or 31) bits for the actual payload.  But
 * since we need to represent different types of unboxed values, we
 * use the entire least significant byte to represent the type tag for
 * unboxed values. This gives us the full tag space:
 *
 *    [n bytes][xxxxxxx0] => boxed object
 *    [n bytes][00000001] => unboxed nil
 *    [n bytes][00000011] => unboxed bool
 *    [n bytes][00000101] => unboxed char
 *    [n bytes][00000111] => unboxed int
 *    [n bytes][00001001] => unboxed symbol
 *    [n bytes][00001011] => unboxed keyword
 *    [n bytes][00001101] => unboxed string
 *
 * where n = 7 for 64 bit systems and n = 3 for 32 bit systems.
 *
 * The first n bytes then are the actual payload and represent the
 * unboxed value. In the case of int-like objects (bool,char,int), you
 * obtain the value by right-shifting 1 byte. In the case of
 * string-like objects (symbol,keyword,string), each byte in the
 * payload encoded one character, up to 7 (or 3) bytes. If the
 * string-like object has more chars, then it cannot be unboxed. If it
 * has exactly that amount of chars, the terminating NUL byte is left
 * implicit. If it has fewer chars, the remaining ones are NUL bytes.
 */
typedef uintptr_t LISHP_Value;

/*****************************************************************************/
/*                                 CONSTANTS                                 */
/*****************************************************************************/

constexpr LISHP_Value LISHP_VALUE_INVALID = (LISHP_Value)0;

constexpr LISHP_Value                  LISHP_VALUE_UNBOXED_MASK = 0b00000001;
constexpr LISHP_Value                  LISHP_VALUE_TAG_MASK     = 0b11111111;
[[maybe_unused]] constexpr LISHP_Value LISHP_VALUE_PAYLOAD_MASK =
  ~LISHP_VALUE_TAG_MASK;

[[maybe_unused]] constexpr int LISHP_VALUE_NUM_TAG_BYTES = 1;
[[maybe_unused]] constexpr int LISHP_VALUE_NUM_TAG_BITS =
  8 * LISHP_VALUE_NUM_TAG_BYTES;
[[maybe_unused]] constexpr int LISHP_VALUE_NUM_PAYLOAD_BYTES =
  sizeof(LISHP_Value) - LISHP_VALUE_NUM_TAG_BITS;
[[maybe_unused]] constexpr int LISHP_VALUE_NUM_PAYLOAD_BITS =
  8 * LISHP_VALUE_NUM_PAYLOAD_BYTES;

/*****************************************************************************/
/*                                 CONVERSION                                */
/*****************************************************************************/

[[nodiscard, maybe_unused]] static inline LISHP_Value
LISHP_Value_MakeBoxed(LISHP_Object* obj)
{
  if (!obj) return LISHP_VALUE_INVALID;

  assert(((uintptr_t)obj & LISHP_VALUE_UNBOXED_MASK) == 0);
  return (LISHP_Value)obj;
}

#endif /* LISHP_VALUE_H */
