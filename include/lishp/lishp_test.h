#ifndef LISHP_TEST_H
#define LISHP_TEST_H

#include "lishp/lishp_alloc.h"
#include "lishp/lishp_prelude.h"
#include "lishp/lishp_test__macos.h"

#include <assert.h>

/*****************************************************************************/
/*                                   MACROS                                  */
/*****************************************************************************/

#define LISHP_TEST(Suite, Name)                                                \
  static void LISHP_TEST_REGISTER__##Suite##__##Name(                          \
    [[maybe_unused]] LISHP_TestContext* ctx)                                   \
  {                                                                            \
  }                                                                            \
                                                                               \
  [[maybe_unused]] LISHP_TEST_SECTION_ATTR static LISHP_TestRegisterFunction   \
    LISHP_TEST_ENTRY__##Suite##__##Name =                                      \
      LISHP_TEST_REGISTER__##Suite##__##Name;                                  \
                                                                               \
  [[maybe_unused]] static void LISHP_TEST__##Suite##__##Name(                  \
    [[maybe_unused]] LISHP_TestContext* LISHP_TEST_CONTEXT,                    \
    [[maybe_unused]] LISHP_Context*     ctx)

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_TestContext LISHP_TestContext;

typedef void (*LISHP_TestRegisterFunction)(LISHP_TestContext*);

/**
 * @brief Context used during execution of all the test cases
 */
struct LISHP_TestContext
{
  LISHP_Allocator* alloc;
};

/*****************************************************************************/
/*                                  GLOBALS                                  */
/*****************************************************************************/

extern LISHP_TestRegisterFunction __start_test_array[] LISHP_TEST_SECTION_START;
extern LISHP_TestRegisterFunction __stop_test_array[] LISHP_TEST_SECTION_STOP;

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

/**
 * @brief Initializer for the test context
 */
[[nodiscard]] LISHP_TestContext*
LISHP_TestContext_Initialize(LISHP_TestContext* ctx, LISHP_Allocator* alloc);
/**
 * @brief Finalizer for the test context
 */
[[nodiscard]] LISHP_TestContext*
LISHP_TestContext_Finalize(LISHP_TestContext* ctx);

/**
 * @returns A freshly created test-context or NULL on failure
 */
[[nodiscard, maybe_unused]] static inline LISHP_TestContext*
LISHP_TestContext_Create(LISHP_Allocator* alloc)
{
  assert(alloc);
  return LISHP_Allocator_Construct(alloc, LISHP_TestContext_Initialize, alloc);
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

/*****************************************************************************/
/*                                    MAIN                                   */
/*****************************************************************************/

/**
 * @brief Implementation for the main function
 */
[[nodiscard]] int
LISHP_TestMain(LISHP_Allocator* alloc, int argc, char* argv[]);

#endif /* LISHP_TEST_H */
