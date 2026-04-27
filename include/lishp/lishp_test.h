#ifndef LISHP_TEST_H
#define LISHP_TEST_H

#include "lishp/lishp_prelude.h"

/*****************************************************************************/
/*                                   MACROS                                  */
/*****************************************************************************/

#define LISHP_TEST(Suite, Name)                                                \
  [[maybe_unused]] static void LISH_TEST__##Suite##__##Name(                   \
    [[maybe_unused]] LISHP_TestContext* LISHP_TEST_CONTEXT,                    \
    [[maybe_unused]] LISHP_Context*     LISHP_CONTEXT)

/*****************************************************************************/
/*                                   TYPES                                   */
/*****************************************************************************/

typedef struct LISHP_TestContext LISHP_TestContext;

/*****************************************************************************/
/*                                    MAIN                                   */
/*****************************************************************************/

[[nodiscard]] int
LISHP_TestMain(int argc, char* argv[]);

#endif /* LISHP_TEST_H */
