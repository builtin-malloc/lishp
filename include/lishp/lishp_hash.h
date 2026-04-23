#ifndef LISHP_HASH_H
#define LISHP_HASH_H

#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/
/*                               HASH FUNCTIONS                              */
/*****************************************************************************/

[[nodiscard]] uint64_t
LISHP_Hash_fnv1a(size_t len, const char* data);

#endif /* LISHP_HASH_H */
