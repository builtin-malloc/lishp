#ifndef LISHP_TEST_H
#define LISHP_TEST_H

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_test__macos.h"

#include <assert.h>
#include <stdio.h>

/*****************************************************************************/
/*                                   MACROS                                  */
/*****************************************************************************/

#define LISHP_TEST__TEMPLATE(NeedsContext, Suite, Name, CtxDecl)               \
  static void LISHP_TEST__##Suite##__##Name(                                   \
    LISHP_TestContext*, const LISHP_TestRegistry_Entry*, CtxDecl);             \
                                                                               \
  static void LISHP_TEST_REGISTER__##Suite##__##Name(LISHP_TestContext* ctx)   \
  {                                                                            \
    LISHP_TestContext_Register(                                                \
      ctx, NeedsContext, #Suite, #Name, LISHP_TEST__##Suite##__##Name);        \
  }                                                                            \
                                                                               \
  LISHP_TEST_SECTION_ATTR static LISHP_TestRegisterFunction                    \
    LISHP_TEST_ENTRY__##Suite##__##Name =                                      \
      LISHP_TEST_REGISTER__##Suite##__##Name;                                  \
                                                                               \
  static void LISHP_TEST__##Suite##__##Name(                                   \
    [[maybe_unused]] LISHP_TestContext*              LISHP_TEST_CONTEXT,       \
    [[maybe_unused]] const LISHP_TestRegistry_Entry* LISHP_TEST_ENTRY,         \
    CtxDecl)

/**
 * @brief Defines a test that uses a fresh LISHP context
 */
#define LISHP_TEST_WITH_CONTEXT(Suite, Name, CtxDecl)                          \
  LISHP_TEST__TEMPLATE(true, Suite, Name, CtxDecl)
/**
 * @brief Defines a test
 */
#define LISHP_TEST(Suite, Name)                                                \
  LISHP_TEST__TEMPLATE(false, Suite, Name, [[maybe_unused]] LISHP_Context*)

/**
 * @brief Helper for the assertion macros
 */
#define LISHP_TEST_ASSERT__TEMPLATE(Cond, Detail)                              \
  do {                                                                         \
    LISHP_TestContext_RecordAssert(LISHP_TEST_CONTEXT);                        \
    if (!(Cond)) {                                                             \
      LISHP_TestContext_RecordAssertFailure(                                   \
        LISHP_TEST_CONTEXT, __FILE__, __LINE__, Detail, LISHP_TEST_ENTRY);     \
    }                                                                          \
  } while (false)

/**
 * @brief Asserts a boolean is true
 */
#define LISHP_TEST_ASSERT_TRUE(Cond)                                           \
  LISHP_TEST_ASSERT__TEMPLATE(Cond, #Cond " to be true")
/**
 * @brief Asserts a boolean is false
 */
#define LISHP_TEST_ASSERT_FALSE(Cond)                                          \
  LISHP_TEST_ASSERT__TEMPLATE(!(Cond), #Cond " to be false")
/**
 * @brief Asserts a pointer is not a NULL pointer
 */
#define LISHP_TEST_ASSERT_NOT_NULL(Cond)                                       \
  LISHP_TEST_ASSERT__TEMPLATE(Cond, #Cond " to not be NULL")
/**
 * @brief Asserts a pointer is a NULL pointer
 */
#define LISHP_TEST_ASSERT_NULL(Cond)                                           \
  LISHP_TEST_ASSERT__TEMPLATE(!(Cond), #Cond " to be NULL")
/**
 * @brief Asserts two values to be equal
 */
#define LISHP_TEST_ASSERT_EQUAL(A, B)                                          \
  LISHP_TEST_ASSERT__TEMPLATE((A) == (B), #A " to equal " #B)
/**
 * @brief Asserts two values to not be equal
 */
#define LISHP_TEST_ASSERT_NOT_EQUAL(A, B)                                      \
  LISHP_TEST_ASSERT__TEMPLATE((A) != (B), #A " to not equal " #B)

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_TestContext         LISHP_TestContext;
typedef struct LISHP_TestRegistry        LISHP_TestRegistry;
typedef struct LISHP_TestRegistry_Entry  LISHP_TestRegistry_Entry;
typedef struct LISHP_TestSummary         LISHP_TestSummary;
typedef struct LISHP_TestSummary_Failure LISHP_TestSummary_Failure;

typedef void (*LISHP_TestRegisterFunction)(LISHP_TestContext*);
typedef void (*LISHP_TestFunction)(LISHP_TestContext*,
                                   const LISHP_TestRegistry_Entry*,
                                   LISHP_Context*);

/**
 * @brief Context used during execution of all the test cases
 */
struct LISHP_TestContext
{
  LISHP_TestRegistry* registry;
  LISHP_TestSummary*  summary;
  LISHP_Allocator*    alloc;
};

/**
 * @brief Holds all tests registered to run at some point
 */
struct LISHP_TestRegistry
{
  size_t                    max_entries;
  size_t                    num_entries;
  LISHP_TestRegistry_Entry* entries;
  LISHP_Allocator*          alloc;
};

/**
 * @brief One entry in the test-registry
 */
struct LISHP_TestRegistry_Entry
{
  bool               needs_context;
  const char*        suite;
  const char*        name;
  LISHP_TestFunction func;
};

/**
 * @brief Summarizes the results of running the tests
 */
struct LISHP_TestSummary
{
  size_t                     num_tests;
  size_t                     num_tests_failed;
  size_t                     num_asserts;
  size_t                     num_asserts_failed;
  size_t                     max_failures;
  size_t                     num_failures;
  LISHP_TestSummary_Failure* failures;
  LISHP_Allocator*           alloc;
};

/**
 * @brief Details why a test failed
 */
struct LISHP_TestSummary_Failure
{
  const char*                     file;
  int                             line;
  const char*                     detail;
  const LISHP_TestRegistry_Entry* entry;
};

/*****************************************************************************/
/*                                  GLOBALS                                  */
/*****************************************************************************/

/**
 * @brief Start of the custom section with the test-register functions
 */
extern LISHP_TestRegisterFunction __start_test_array[] LISHP_TEST_SECTION_START;
/**
 * @brief End of the custom section with the test-register functions
 */
extern LISHP_TestRegisterFunction __stop_test_array[] LISHP_TEST_SECTION_STOP;

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

/* CONTEXT *******************************************************************/

/**
 * @brief Initializer for the test context
 */
[[nodiscard]] LISHP_TestContext*
LISHP_TestContext_Initialize(LISHP_TestContext* ctx,
                             LISHP_Allocator*   alloc,
                             size_t             max_tests);
/**
 * @brief Finalizer for the test context
 */
[[nodiscard]] LISHP_TestContext*
LISHP_TestContext_Finalize(LISHP_TestContext* ctx);

/**
 * @returns A freshly created test-context or NULL on failure
 */
[[nodiscard, maybe_unused]] static inline LISHP_TestContext*
LISHP_TestContext_Create(LISHP_Allocator* alloc, size_t max_test)
{
  assert(alloc);
  return LISHP_Allocator_Construct(
    alloc, LISHP_TestContext_Initialize, alloc, max_test);
}

/**
 * @brief Destroys the test-context
 */
[[maybe_unused]] static inline LISHP_TestContext*
LISHP_TestContext_Destroy(LISHP_TestContext* ctx)
{
  if (!ctx) return nullptr;
  return LISHP_Allocator_Destruct(ctx->alloc, LISHP_TestContext_Finalize, ctx);
}

/* REGISTRY ******************************************************************/

/**
 * @brief Initializer for the test registry
 */
[[nodiscard]] LISHP_TestRegistry*
LISHP_TestRegistry_Initialize(LISHP_TestRegistry* reg,
                              LISHP_Allocator*    alloc,
                              size_t              max_entries);
/**
 * @brief Finalizer for the test registry
 */
[[nodiscard]] LISHP_TestRegistry*
LISHP_TestRegistry_Finalize(LISHP_TestRegistry* reg);

/**
 * @returns A freshly created test-registry or NULL on failure
 */
[[nodiscard, maybe_unused]] static inline LISHP_TestRegistry*
LISHP_TestRegistry_Create(LISHP_Allocator* alloc, size_t max_entries)
{
  assert(alloc);
  return LISHP_Allocator_Construct(
    alloc, LISHP_TestRegistry_Initialize, alloc, max_entries);
}

/**
 * @brief Destroys the test-registry
 */
[[maybe_unused]] static inline LISHP_TestRegistry*
LISHP_TestRegistry_Destroy(LISHP_TestRegistry* reg)
{
  if (!reg) return nullptr;
  return LISHP_Allocator_Destruct(reg->alloc, LISHP_TestRegistry_Finalize, reg);
}

/* SUMMARY *******************************************************************/

/**
 * @brief Initializer for the test summary
 */
[[nodiscard]] LISHP_TestSummary*
LISHP_TestSummary_Initialize(LISHP_TestSummary* sum,
                             LISHP_Allocator*   alloc,
                             size_t             max_failures);
/**
 * @brief Finalizer for the test summary
 */
[[nodiscard]] LISHP_TestSummary*
LISHP_TestSummary_Finalize(LISHP_TestSummary* sum);

/**
 * @returns A freshly created test-summary or NULL on failure
 */
[[nodiscard, maybe_unused]] static inline LISHP_TestSummary*
LISHP_TestSummary_Create(LISHP_Allocator* alloc, size_t max_failures)
{
  assert(alloc);
  return LISHP_Allocator_Construct(
    alloc, LISHP_TestSummary_Initialize, alloc, max_failures);
}

/**
 * @brief Destroys the test-summary
 */
[[maybe_unused]] static inline LISHP_TestSummary*
LISHP_TestSummary_Destroy(LISHP_TestSummary* sum)
{
  if (!sum) return nullptr;
  return LISHP_Allocator_Destruct(sum->alloc, LISHP_TestSummary_Finalize, sum);
}

/*****************************************************************************/
/*                                CONTEXT API                                */
/*****************************************************************************/

/**
 * @brief Adds the test to the registry of known tests
 */
void
LISHP_TestRegistry_Register(LISHP_TestRegistry* reg,
                            bool                needs_context,
                            const char*         suite,
                            const char*         name,
                            LISHP_TestFunction  func);

/**
 * @brief Adds the test to the registry of known tests
 */
[[maybe_unused]] static inline void
LISHP_TestContext_Register(LISHP_TestContext* ctx,
                           bool               needs_context,
                           const char*        suite,
                           const char*        name,
                           LISHP_TestFunction func)
{
  assert(ctx);
  assert(suite);
  assert(name);
  assert(func);

  LISHP_TestRegistry_Register(ctx->registry, needs_context, suite, name, func);
}

/**
 * @brief Record that an assert was executed
 */
void
LISHP_TestSummary_RecordAssert(LISHP_TestSummary* sum);
/**
 * @brief Record that an assert failed
 */
void
LISHP_TestSummary_RecordAssertFailure(LISHP_TestSummary*              sum,
                                      const char*                     file,
                                      int                             line,
                                      const char*                     detail,
                                      const LISHP_TestRegistry_Entry* entry);

/**
 * @brief Record that an assert was executed
 */
[[maybe_unused]] static inline void
LISHP_TestContext_RecordAssert(LISHP_TestContext* ctx)
{
  assert(ctx);
  LISHP_TestSummary_RecordAssert(ctx->summary);
}

/**
 * @brief Record that an assert failed
 */
[[maybe_unused]] static inline void
LISHP_TestContext_RecordAssertFailure(LISHP_TestContext*              ctx,
                                      const char*                     file,
                                      int                             line,
                                      const char*                     detail,
                                      const LISHP_TestRegistry_Entry* entry)
{
  assert(ctx);
  assert(file);
  assert(detail);

  LISHP_TestSummary_RecordAssertFailure(
    ctx->summary, file, line, detail, entry);
}

[[nodiscard, maybe_unused]] static inline const LISHP_TestRegistry_Entry*
LISHP_TestContext_EntriesBegin(const LISHP_TestContext* ctx)
{
  assert(ctx);
  assert(ctx->registry);
  assert(ctx->registry->entries);
  return ctx->registry->entries;
}

[[nodiscard, maybe_unused]] static inline const LISHP_TestRegistry_Entry*
LISHP_TestContext_EntriesEnd(const LISHP_TestContext* ctx)
{
  assert(ctx);
  assert(ctx->registry);
  assert(ctx->registry->entries);
  return ctx->registry->entries + ctx->registry->num_entries;
}

void
LISHP_TestSummary_PrintFailures(const LISHP_TestSummary* sum, FILE* stream);
void
LISHP_TestSummary_PrintTotals(const LISHP_TestSummary* sum, FILE* stream);

[[nodiscard, maybe_unused]] static inline bool
LISHP_TestContext_HasAnyFailures(const LISHP_TestContext* ctx)
{
  assert(ctx);
  assert(ctx->summary);
  return ctx->summary->num_asserts_failed > 0;
}

[[maybe_unused]] static inline void
LISHP_TestContext_PrintFailures(const LISHP_TestContext* ctx, FILE* stream)
{
  assert(ctx);
  LISHP_TestSummary_PrintFailures(ctx->summary, stream);
}

[[maybe_unused]] static inline void
LISHP_TestContext_PrintSummary(const LISHP_TestContext* ctx, FILE* stream)
{
  assert(ctx);
  LISHP_TestSummary_PrintTotals(ctx->summary, stream);
}

/*****************************************************************************/
/*                                    MAIN                                   */
/*****************************************************************************/

/**
 * @brief Implementation for the main function
 */
[[nodiscard]] int
LISHP_TestMain(LISHP_Allocator* alloc, int argc, char* argv[]);

#endif /* LISHP_TEST_H */
