/**
 * @file lishp_alloc.h
 * @brief Allocator Interface
 */

#ifndef LISHP_ALLOC_H
#define LISHP_ALLOC_H

#include "lishp/lishp_prelude.h"

#include <stddef.h>
#include <string.h>

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

/**
 * @brief Interface for `malloc`-like functions
 * @note Implementations may assert the size is not 0
 */
typedef void* (*LISHP_Allocator_MallocFunction)(void*, size_t size);
/**
 * @brief Interface for `free`-like functions
 * @note Implementations may assert the pointer is not NULL
 */
typedef void  (*LISHP_Allocator_FreeFunction)(void*, void*);
/**
 * @brief Interface for `realloc`-like functions
 * @note Implementations may assert the pointer is not NULL
 * @note Implementations may assert the size is not 0
 */
typedef void* (*LISHP_Allocator_ReallocFunction)(void*, void*, size_t);

/**
 * @brief Virtual table for an allocator instance
 */
typedef struct LISHP_Allocator_VirtualTable
{
  LISHP_Allocator_MallocFunction  malloc_function;
  LISHP_Allocator_FreeFunction    free_function;
  LISHP_Allocator_ReallocFunction realloc_function;
} LISHP_Allocator_VirtualTable;

/**
 * @brief Interface for memory allocators
 */
struct LISHP_Allocator
{
  void*                               context;
  const LISHP_Allocator_VirtualTable* virtual_table;
};

/*****************************************************************************/
/*                                    API                                    */
/*****************************************************************************/

/**
 * @brief Allocates an object and initializes it
 * @note The initialization function must return the initialized object with the
 *       correct type
 * @note The initialization function may assume the memory is 0-initialized
 */
#define LISHP_Allocator_Construct(Alloc, Init, ...)                            \
  Init(LISHP_Allocator_MallocZeroed(                                           \
    Alloc, sizeof(*(Init(nullptr __VA_OPT__(, ) __VA_ARGS__)))) __VA_OPT__(, ) \
         __VA_ARGS__)
/**
 * @brief Finalizes an object and frees it
 * @note The finalization function must return the finalized object
 */
#define LISHP_Allocator_Destruct(Alloc, Fini, Ptr)                             \
  LISHP_Allocator_Free(Alloc, Fini(Ptr));

/**
 * @brief Allocates using the allocators `malloc` function
 * @returns the allocated memory
 * @note If the allocator is NULL or it has no `malloc`, this returns NULL
 * @note If the size is 0, this returns NULL
 */
[[nodiscard]] static inline void*
LISHP_Allocator_Malloc(LISHP_Allocator* alloc, size_t size)
{
  if (!alloc) return nullptr;
  if (!alloc->virtual_table) return nullptr;
  if (!alloc->virtual_table->malloc_function) return nullptr;
  if (size == 0) return nullptr;
  return alloc->virtual_table->malloc_function(alloc->context, size);
}

/**
 * @brief Deallocates using the allocators `free` function
 * @returns a NULL pointer
 * @note If the allocator is NULL or it has no `free`, this returns the input
 * @note If the input is NULL, this does nothing
 */
[[maybe_unused]] static inline void*
LISHP_Allocator_Free(LISHP_Allocator* alloc, void* ptr)
{
  if (!alloc) return ptr;
  if (!alloc->virtual_table) return ptr;
  if (!alloc->virtual_table->free_function) return ptr;
  if (!ptr) return nullptr;
  alloc->virtual_table->free_function(alloc->context, ptr);
  return nullptr;
}

/**
 * @brief Reallocates using the allocators `realloc` function
 * @returns the reallocated memory
 * @note If the allocator is NULL or it has no `realloc`, this returns NULL
 * @note If the input is NULL, this calls the allocators `malloc` instead
 * @note If the size is 0, this returns NULL
 */
[[nodiscard, maybe_unused]] static inline void*
LISHP_Allocator_Realloc(LISHP_Allocator* alloc, void* ptr, size_t size)
{
  if (!alloc) return nullptr;
  if (!alloc->virtual_table) return nullptr;
  if (!alloc->virtual_table->realloc_function) return nullptr;
  if (size == 0) return nullptr;
  if (!ptr) return LISHP_Allocator_Malloc(alloc, size);
  return alloc->virtual_table->realloc_function(alloc->context, ptr, size);
}

/**
 * @brief Allocates memory like the `malloc` function and fills it with zeroes
 */
[[nodiscard, maybe_unused]] static inline void*
LISHP_Allocator_MallocZeroed(LISHP_Allocator* alloc, size_t size)
{
  auto ptr = LISHP_Allocator_Malloc(alloc, size);
  if (ptr) memset(ptr, 0, size);
  return ptr;
}

/*****************************************************************************/
/*                             DEFAULT ALLOCATOR                             */
/*****************************************************************************/

/**
 * @brief Virtual table for the global heap allocator
 */
extern const LISHP_Allocator_VirtualTable LISHP_HeapAllocator_virtual_table;

/**
 * @returns the global `malloc`-based heap allocator
 */
[[nodiscard]] LISHP_Allocator*
LISHP_HeapAllocator();

#endif /* LISHP_ALLOC_H */
