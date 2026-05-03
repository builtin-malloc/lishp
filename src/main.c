#include "lishp/lishp_alloc.h"

#include "repl.h"

int
main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  return repl(LISHP_HeapAllocator());
}
