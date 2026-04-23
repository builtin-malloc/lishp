#include "lishp/lishp_hash.h"

#include <assert.h>

/*****************************************************************************/
/*                               HASH FUNCTIONS                              */
/*****************************************************************************/

[[nodiscard]] uint64_t
LISHP_Hash_fnv1a(size_t len, const char* data)
{
  assert(data);
  uint64_t hash = 1469598103934665603ULL;
  for (size_t i = 0; i < len; i++) {
    hash ^= (unsigned char)data[i];
    hash *= 1099511628211ULL;
  }
  return hash;
}
