#ifndef LISHP_TEST_H
#define LISHP_TEST_H

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_test__macos.h"

#include <assert.h>

/*****************************************************************************/
/*                                   MACROS                                  */
/*****************************************************************************/

/**
 * @brief Defines a test
 */
#define LISHP_TEST(Suite, Name)                                                \
  static void LISHP_TEST__##Suite##__##Name(LISHP_TestContext*,                \
                                            LISHP_Context*);                   \
                                                                               \
  static void LISHP_TEST_REGISTER__##Suite##__##Name(LISHP_TestContext* ctx)   \
  {                                                                            \
    LISHP_TestContext_Register(                                                \
      ctx, #Suite, #Name, LISHP_TEST__##Suite##__##Name);                      \
  }                                                                            \
                                                                               \
  LISHP_TEST_SECTION_ATTR static LISHP_TestRegisterFunction                    \
    LISHP_TEST_ENTRY__##Suite##__##Name =                                      \
      LISHP_TEST_REGISTER__##Suite##__##Name;                                  \
                                                                               \
  static void LISHP_TEST__##Suite##__##Name(                                   \
    [[maybe_unused]] LISHP_TestContext* LISHP_TEST_CONTEXT,                    \
    [[maybe_unused]] LISHP_Context*     ctx)

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_TestContext        LISHP_TestContext;
typedef struct LISHP_TestRegistry       LISHP_TestRegistry;
typedef struct LISHP_TestRegistry_Entry LISHP_TestRegistry_Entry;

typedef void (*LISHP_TestRegisterFunction)(LISHP_TestContext*);
typedef void (*LISHP_TestFunction)(LISHP_TestContext*, LISHP_Context*);

/**
 * @brief Context used during execution of all the test cases
 */
struct LISHP_TestContext
{
  LISHP_TestRegistry* registry;
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
  const char*        suite;
  const char*        name;
  LISHP_TestFunction func;
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

/*****************************************************************************/
/*                                CONTEXT API                                */
/*****************************************************************************/

/**
 * @brief Adds the test to the registry of known tests
 */
void
LISHP_TestRegistry_Register(LISHP_TestRegistry* reg,
                            const char*         suite,
                            const char*         name,
                            LISHP_TestFunction  func);

/**
 * @brief Adds the test to the registry of known tests
 */
[[maybe_unused]] static inline void
LISHP_TestContext_Register(LISHP_TestContext* ctx,
                           const char*        suite,
                           const char*        name,
                           LISHP_TestFunction func)
{
  assert(ctx);
  assert(suite);
  assert(name);
  assert(func);

  LISHP_TestRegistry_Register(ctx->registry, suite, name, func);
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
