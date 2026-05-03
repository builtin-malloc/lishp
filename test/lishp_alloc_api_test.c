#include "lishp/lishp_alloc.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_test.h"

#include <assert.h>
#include <stdio.h>

/*****************************************************************************/
/*                              test allocators                              */
/*****************************************************************************/

static char         test_context;
static const size_t test_malloc_size = 42;
static char         test_malloc_result;
static const size_t test_realloc_size = 69;
static char         test_realloc_result;

static void*
test_malloc__impl(void* ctx, size_t size)
{
  if (ctx != &test_context) return nullptr;
  if (size != test_malloc_size) return nullptr;
  return &test_malloc_result;
}

static void*
test_realloc__impl(void* ctx, void* ptr, size_t size)
{
  if (ctx != &test_context) return nullptr;
  if (ptr != &test_malloc_result) return nullptr;
  if (size != test_realloc_size) return nullptr;
  return &test_realloc_result;
}

static void
test_free__impl(void* ctx, void* ptr)
{
  // TODO: Using assert here is not actually what I want to do.
  //       We should set up the context to be something more useful.
  assert(ctx == &test_context);
  assert(ptr == &test_malloc_result);
}

static const LISHP_Allocator_VirtualTable null_vtable = { 0 };
static LISHP_Allocator null_allocator = { .virtual_table = &null_vtable };

static const LISHP_Allocator_VirtualTable test_vtable = {
  .malloc_function  = test_malloc__impl,
  .realloc_function = test_realloc__impl,
  .free_function    = test_free__impl,
};
static LISHP_Allocator test_allocator = {
  .context       = &test_context,
  .virtual_table = &test_vtable,
};

/*****************************************************************************/
/*                                   malloc                                  */
/*****************************************************************************/

LISHP_TEST(alloc, malloc)
{
  auto ptr = LISHP_Allocator_Malloc(&test_allocator, test_malloc_size);
  LISHP_TEST_ASSERT_EQUAL(ptr, &test_malloc_result);
}

LISHP_TEST(alloc, malloc__null_allocators)
{
  LISHP_TEST_ASSERT_NULL(LISHP_Allocator_Malloc(nullptr, 420));
  LISHP_TEST_ASSERT_NULL(LISHP_Allocator_Malloc(&null_allocator, 0x69));
}

LISHP_TEST(alloc, malloc__zero_size)
{
  auto alloc = LISHP_HeapAllocator();
  LISHP_TEST_ASSERT_NULL(LISHP_Allocator_Malloc(alloc, 0));
}

/*****************************************************************************/
/*                                  realloc                                  */
/*****************************************************************************/

LISHP_TEST(alloc, realloc)
{
  auto ptr = LISHP_Allocator_Malloc(&test_allocator, test_malloc_size);
  ptr      = LISHP_Allocator_Realloc(&test_allocator, ptr, test_realloc_size);
  LISHP_TEST_ASSERT_EQUAL(ptr, &test_realloc_result);
}

LISHP_TEST(alloc, realloc__null_allocators)
{
  char obj;
  LISHP_TEST_ASSERT_NULL(LISHP_Allocator_Realloc(nullptr, &obj, 420));
  LISHP_TEST_ASSERT_NULL(LISHP_Allocator_Realloc(&null_allocator, &obj, 0x69));
}

LISHP_TEST(alloc, realloc__zero_size)
{
  char obj;
  auto alloc = LISHP_HeapAllocator();
  LISHP_TEST_ASSERT_NULL(LISHP_Allocator_Realloc(alloc, &obj, 0));
}

LISHP_TEST(alloc, realloc__null_ptr)
{
  auto ptr =
    LISHP_Allocator_Realloc(&test_allocator, nullptr, test_malloc_size);
  LISHP_TEST_ASSERT_EQUAL(ptr, &test_malloc_result);
}

/*****************************************************************************/
/*                                    free                                   */
/*****************************************************************************/

LISHP_TEST(alloc, free)
{
  auto ptr = LISHP_Allocator_Malloc(&test_allocator, test_malloc_size);
  LISHP_TEST_ASSERT_NULL(LISHP_Allocator_Free(&test_allocator, ptr));
}

LISHP_TEST(alloc, free__null_allocators)
{
  char obj;
  LISHP_TEST_ASSERT_EQUAL(LISHP_Allocator_Free(nullptr, &obj), &obj);
  LISHP_TEST_ASSERT_EQUAL(LISHP_Allocator_Free(&null_allocator, &obj), &obj);
}

LISHP_TEST(alloc, free__null_ptr)
{
  LISHP_TEST_ASSERT_NULL(LISHP_Allocator_Free(&test_allocator, nullptr));
}
