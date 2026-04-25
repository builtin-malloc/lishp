#include "lishp/lishp_hash.h"
#include "lishp/lishp_alloc.h"
#include "lishp/lishp_diag.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_value.h"

#include <assert.h>
#include <inttypes.h>
#include <string.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] bool
LISHP_HashMap_Initialize(LISHP_HashMap* map,
                         size_t         capacity,
                         LISHP_Alloc*   alloc)
{
  assert(map);
  assert(capacity > 0);
  assert(alloc);

  memset(map, 0, sizeof(*map));

  LISHP_HashEntry* entries =
    LISHP_Alloc_Malloc(alloc, capacity * sizeof(*entries));
  if (!entries) {
    // TODO: Print Error
    return false;
  }

  map->max_entries = capacity;
  map->num_entries = 0;
  map->num_free    = capacity;
  map->entries     = entries;
  map->alloc       = alloc;

  for (size_t i = 0; i < capacity; ++i) {
    entries[i].key   = LISHP_VALUE_INVALID;
    entries[i].value = LISHP_VALUE_INVALID;
    entries[i].state = LISHP_HASHENTRY_STATE_FREE;
  }

  return true;
}

void
LISHP_HashMap_Finalize(LISHP_HashMap* map)
{
  assert(map);

  if (map->entries) {
    LISHP_Alloc_Free(map->alloc, map->entries);
  }
}

void
LISHP_HashMap_Move(LISHP_HashMap* dst, LISHP_HashMap* src)
{
  assert(dst);
  assert(src);

  LISHP_HashMap_Finalize(dst);

  dst->max_entries = src->max_entries;
  dst->num_entries = src->num_entries;
  dst->num_free    = src->num_free;
  dst->entries     = src->entries;
  dst->alloc       = src->alloc;

  src->max_entries = 0;
  src->num_entries = 0;
  src->num_free    = 0;
  src->entries     = nullptr;
  src->alloc       = nullptr;
}

/*****************************************************************************/
/*                                  HELPERS                                  */
/*****************************************************************************/

[[nodiscard]] bool
LISHP_HashMap_IsFull(const LISHP_HashMap* map)
{
  assert(map);

  auto max_entries = map->max_entries;
  auto num_entries = map->num_entries;
  auto num_deleted = max_entries - num_entries - map->num_free;

  auto density_reached       = num_entries >= 0.7 * max_entries;
  auto fragmentation_reached = num_deleted >= 0.2 * max_entries;

  return density_reached || fragmentation_reached;
}

[[nodiscard]] bool
LISHP_HashMap_Resize(LISHP_HashMap* map, size_t new_capacity)
{
  assert(map);
  assert(new_capacity > 0);

  auto alloc        = map->alloc;
  auto old_entries  = map->entries;
  auto old_capacity = map->max_entries;

  LISHP_HashMap tmp_map;
  if (!LISHP_HashMap_Initialize(&tmp_map, new_capacity, alloc)) return false;

  for (size_t i = 0; i < old_capacity; ++i) {
    auto entry = &old_entries[i];
    auto key   = entry->key;
    auto val   = entry->value;
    auto state = entry->state;

    if (state != LISHP_HASHENTRY_STATE_TAKEN) continue;

    if (LISHP_HashMap_Insert(&tmp_map, key, val) == LISHP_VALUE_INVALID) {
      // TODO: Print Error
      LISHP_HashMap_Finalize(&tmp_map);
      return false;
    }
  }

  LISHP_HashMap_Move(map, &tmp_map);
  return true;
}

static LISHP_HashEntry*
LISHP_HashMap_GetEntry_impl(const LISHP_HashMap* map, LISHP_Value key)
{
  assert(map);
  assert(key != LISHP_VALUE_INVALID);

  auto hash = LISHP_Value_GetHash(key);

  for (size_t i = 0; i < map->max_entries; ++i) {
    auto index = (hash + i) % map->max_entries;
    auto entry = &map->entries[index];

    switch (entry->state) {
      case LISHP_HASHENTRY_STATE_TAKEN:
        if (LISHP_Value_AreEqual(key, entry->key)) return entry;
        break;
      case LISHP_HASHENTRY_STATE_FREE   : return nullptr;
      case LISHP_HASHENTRY_STATE_DELETED: break;
    }
  }

  return nullptr;
}

[[nodiscard]] const LISHP_HashEntry*
LISHP_HashMap_GetEntry(const LISHP_HashMap* map, LISHP_Value key)
{
  return LISHP_HashMap_GetEntry_impl(map, key);
}

[[nodiscard]] LISHP_HashEntry*
LISHP_HashMap_GetEntry_mut(LISHP_HashMap* map, LISHP_Value key)
{
  return LISHP_HashMap_GetEntry_impl(map, key);
}

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

LISHP_Value
LISHP_HashMap_Insert(LISHP_HashMap* map, LISHP_Value key, LISHP_Value value)
{
  assert(map);
  assert(key != LISHP_VALUE_INVALID);

  if (LISHP_HashMap_IsFull(map)) {
    if (!LISHP_HashMap_Resize(map, 2 * map->max_entries)) {
      // TODO: Print error
      return LISHP_VALUE_INVALID;
    }
  }

  auto hash = LISHP_Value_GetHash(key);

  LISHP_HashEntry* candidate = nullptr;

  for (size_t i = 0; i < map->max_entries; ++i) {
    auto index = (hash + i) % map->max_entries;
    auto entry = &map->entries[index];

    switch (entry->state) {
      case LISHP_HASHENTRY_STATE_TAKEN:
        if (LISHP_Value_AreEqual(key, entry->key)) {
          entry->value = value;
          return key;
        }
        break;
      case LISHP_HASHENTRY_STATE_DELETED:
        if (candidate == nullptr) {
          candidate = entry;
        }
        break;
      case LISHP_HASHENTRY_STATE_FREE:
        if (candidate == nullptr) {
          candidate      = entry;
          map->num_free -= 1;
        }
        goto fill_candidate;
    }
  }

fill_candidate:
  assert(candidate != nullptr);

  map->num_entries += 1;

  candidate->key   = key;
  candidate->value = value;
  candidate->state = LISHP_HASHENTRY_STATE_TAKEN;

  return key;
}

LISHP_Value
LISHP_HashMap_Delete(LISHP_HashMap* map, LISHP_Value key)
{
  assert(map);
  assert(key != LISHP_VALUE_INVALID);

  auto entry = LISHP_HashMap_GetEntry_mut(map, key);
  if (!entry) return LISHP_VALUE_INVALID;

  assert(entry->state == LISHP_HASHENTRY_STATE_TAKEN);

  entry->key   = LISHP_VALUE_INVALID;
  entry->value = LISHP_VALUE_INVALID;
  entry->state = LISHP_HASHENTRY_STATE_DELETED;

  map->num_entries -= 1;

  return key;
}

[[nodiscard]] bool
LISHP_HashMap_Contains(const LISHP_HashMap* map, LISHP_Value key)
{
  assert(map);
  assert(key != LISHP_VALUE_INVALID);
  return LISHP_HashMap_GetEntry(map, key) != nullptr;
}

[[nodiscard]] LISHP_Value
LISHP_HashMap_FindByHash(LISHP_HashMap* map,
                         uint64_t       hash,
                         bool           (*pred)(LISHP_Value key, void* ctx),
                         void*          ctx)
{
  // TODO: This code is now duplicated with the code from the Get functions.
  //       I am not sure if I want to just use this function in the get
  //       functions because of the predicate indirection.

  assert(map);
  assert(pred);

  for (size_t i = 0; i < map->max_entries; ++i) {
    auto index = (hash + i) % map->max_entries;
    auto entry = &map->entries[index];

    switch (entry->state) {
      case LISHP_HASHENTRY_STATE_TAKEN:
        if (LISHP_Value_GetHash(entry->key) == hash && pred(entry->key, ctx))
          return entry->key;
        break;
      case LISHP_HASHENTRY_STATE_FREE   : return LISHP_VALUE_INVALID;
      case LISHP_HASHENTRY_STATE_DELETED: break;
    }
  }

  return LISHP_VALUE_INVALID;
}

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
