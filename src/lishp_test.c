#include "lishp/lishp_test.h"
#include "lishp/lishp_alloc.h"

#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

/* CONTEXT *******************************************************************/

[[nodiscard]] LISHP_TestContext*
LISHP_TestContext_Initialize(LISHP_TestContext* ctx,
                             LISHP_Allocator*   alloc,
                             size_t             max_tests)
{
  if (!ctx) goto failure;

  ctx->registry = LISHP_TestRegistry_Create(alloc, max_tests);
  ctx->summary  = LISHP_TestSummary_Create(alloc, max_tests);
  ctx->alloc    = alloc;

  if (!ctx->registry || !ctx->summary) goto failure;

  return ctx;

failure:
  fprintf(stderr, "!!!FATAL!!! Failed to allocate the test context");
  return LISHP_TestContext_Destroy(ctx);
}

[[nodiscard]] LISHP_TestContext*
LISHP_TestContext_Finalize(LISHP_TestContext* ctx)
{
  if (!ctx) return nullptr;

  ctx->registry = LISHP_TestRegistry_Destroy(ctx->registry);
  ctx->summary  = LISHP_TestSummary_Destroy(ctx->summary);
  ctx->alloc    = nullptr;

  return ctx;
}

/* REGISTRY ******************************************************************/

[[nodiscard]] LISHP_TestRegistry*
LISHP_TestRegistry_Initialize(LISHP_TestRegistry* reg,
                              LISHP_Allocator*    alloc,
                              size_t              max_entries)
{
  if (!reg) goto failure;

  reg->entries =
    LISHP_Allocator_Malloc(alloc, max_entries * sizeof(*reg->entries));
  reg->max_entries = reg->entries ? max_entries : 0;
  reg->num_entries = 0;
  reg->alloc       = alloc;

  if (!reg->entries) goto failure;

  return reg;

failure:
  fprintf(stderr, "!!!WARN!!! Failed to allocate the test registry");
  return LISHP_TestRegistry_Destroy(reg);
}

[[nodiscard]] LISHP_TestRegistry*
LISHP_TestRegistry_Finalize(LISHP_TestRegistry* reg)
{
  if (!reg) return nullptr;

  reg->entries     = LISHP_Allocator_Free(reg->alloc, reg->entries);
  reg->max_entries = 0;
  reg->num_entries = 0;
  reg->alloc       = nullptr;

  return reg;
}

/* REGISTRY ******************************************************************/

[[nodiscard]] LISHP_TestSummary*
LISHP_TestSummary_Initialize(LISHP_TestSummary* sum,
                             LISHP_Allocator*   alloc,
                             size_t             max_failures)
{
  if (!sum) goto failure;

  sum->num_tests          = 0;
  sum->num_tests_failed   = 0;
  sum->num_asserts        = 0;
  sum->num_asserts_failed = 0;
  sum->failures =
    LISHP_Allocator_Malloc(alloc, max_failures * sizeof(*sum->failures));
  sum->max_failures = sum->failures ? max_failures : 0;
  sum->num_failures = 0;
  sum->alloc        = alloc;

  if (!sum->failures) goto failure;

  return sum;

failure:
  fprintf(stderr, "!!!WARN!!! Failed to allocate the test summary");
  return LISHP_TestSummary_Destroy(sum);
}

[[nodiscard]] LISHP_TestSummary*
LISHP_TestSummary_Finalize(LISHP_TestSummary* sum)
{
  if (!sum) return nullptr;

  sum->num_tests          = 0;
  sum->num_tests_failed   = 0;
  sum->num_asserts        = 0;
  sum->num_asserts_failed = 0;
  sum->failures           = LISHP_Allocator_Free(sum->alloc, sum->failures);
  sum->max_failures       = 0;
  sum->num_failures       = 0;
  sum->alloc              = nullptr;

  return sum;
}

/*****************************************************************************/
/*                                CONTEXT API                                */
/*****************************************************************************/

void
LISHP_TestRegistry_Register(LISHP_TestRegistry* reg,
                            const char*         suite,
                            const char*         name,
                            LISHP_TestFunction  func)
{
  assert(reg);
  assert(suite);
  assert(name);
  assert(func);

  if (reg->num_entries >= reg->max_entries) {
    fprintf(stderr,
            "!!!FATAL!!! Tried to register more than all tests? This is an "
            "internal bug!\n");
    abort();
  }

  auto idx   = reg->num_entries;
  auto entry = &reg->entries[idx];

  entry->suite = suite;
  entry->name  = name;
  entry->func  = func;

  reg->num_entries += 1;
}
