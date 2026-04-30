#ifndef LISHP_TEST__MACOS_H
#define LISHP_TEST__MACOS_H

#if defined(__APPLE__) && defined(__MACH__)

#define LISHP_TEST_SECTION_ATTR                                                \
  __attribute__((section("__DATA,__test_array"), used))

#define LISHP_TEST_SECTION_START __asm("section$start$__DATA$__test_array")
#define LISHP_TEST_SECTION_STOP  __asm("section$end$__DATA$__test_array")

#endif

#endif /* LISHP_TEST__MACOS_H */
