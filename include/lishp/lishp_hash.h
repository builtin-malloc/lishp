#ifndef LISHP_HASH_H
#define LISHP_HASH_H

#include "lishp_alloc.h"
#include "lishp_value.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_HashMap   LISHP_HashSet;
typedef struct LISHP_HashMap   LISHP_HashMap;
typedef struct LISHP_HashEntry LISHP_HashEntry;

typedef enum LISHP_HashEntry_State
{
  LISHP_HASHENTRY_STATE_FREE,
  LISHP_HASHENTRY_STATE_TAKEN,
  LISHP_HASHENTRY_STATE_DELETED,
} LISHP_HashEntry_State;

struct LISHP_HashEntry
{
  LISHP_Value           key;
  LISHP_Value           value;
  LISHP_HashEntry_State state;
};

struct LISHP_HashMap
{
  size_t           max_entries;
  size_t           num_entries;
  size_t           num_free;
  LISHP_HashEntry* entries;
  LISHP_Alloc*     alloc;
};

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] bool
LISHP_HashMap_Initialize(LISHP_HashMap* map,
                         size_t         capacity,
                         LISHP_Alloc*   alloc);
void
LISHP_HashMap_Finalize(LISHP_HashMap* map);

[[nodiscard, maybe_unused]] static inline bool
LISHP_HashSet_Initialize(LISHP_HashSet* set,
                         size_t         capacity,
                         LISHP_Alloc*   alloc)
{
  return LISHP_HashMap_Initialize(set, capacity, alloc);
}
[[maybe_unused]] static inline void
LISHP_HashSet_Finalize(LISHP_HashSet* set)
{
  LISHP_HashMap_Finalize(set);
}

void
LISHP_HashMap_Move(LISHP_HashMap* dst, LISHP_HashMap* src);

[[maybe_unused]] static inline void
LISHP_HashSet_Move(LISHP_HashSet* dst, LISHP_HashSet* src)
{
  LISHP_HashMap_Move(dst, src);
}

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

[[nodiscard]] bool
LISHP_HashMap_IsFull(const LISHP_HashMap* map);
[[nodiscard]] bool
LISHP_HashMap_Resize(LISHP_HashMap* map, size_t new_capacity);

[[nodiscard]] const LISHP_HashEntry*
LISHP_HashMap_GetEntry(const LISHP_HashMap* map, LISHP_Value key);
[[nodiscard]] LISHP_HashEntry*
LISHP_HashMap_GetEntry_mut(LISHP_HashMap* map, LISHP_Value key);

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

LISHP_Value
LISHP_HashMap_Insert(LISHP_HashMap* map, LISHP_Value key, LISHP_Value value);
LISHP_Value
LISHP_HashMap_Delete(LISHP_HashMap* map, LISHP_Value key);
[[nodiscard]] bool
LISHP_HashMap_Contains(const LISHP_HashMap* map, LISHP_Value key);

[[maybe_unused]] static inline LISHP_Value
LISHP_HashSet_Insert(LISHP_HashSet* set, LISHP_Value key)
{
  return LISHP_HashMap_Insert(set, key, LISHP_VALUE_INVALID);
}
[[maybe_unused]] static inline LISHP_Value
LISHP_HashSet_Delete(LISHP_HashSet* set, LISHP_Value key)
{
  return LISHP_HashMap_Delete(set, key);
}
[[nodiscard, maybe_unused]] static inline bool
LISHP_HashSet_Contrains(const LISHP_HashSet* set, LISHP_Value key)
{
  return LISHP_HashMap_Contains(set, key);
}

[[nodiscard, maybe_unused]] static inline LISHP_Value
LISHP_HashMap_Get(LISHP_HashMap* map, LISHP_Value key)
{
  auto entry = LISHP_HashMap_GetEntry(map, key);
  if (entry) return entry->value;
  return LISHP_VALUE_INVALID;
}

/*****************************************************************************/
/*                               HASH FUNCTIONS                              */
/*****************************************************************************/

[[nodiscard]] uint64_t
LISHP_Hash_fnv1a(size_t len, const char* data);

#endif /* LISHP_HASH_H */
