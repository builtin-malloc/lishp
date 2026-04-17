#include "lishp/lishp_write.h"
#include "lishp/lishp_alloc.h"

#include <assert.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Writer*
LISHP_Writer_Create(LISHP_Alloc* alloc)
{
  assert(alloc);

  LISHP_Writer* writer = LISHP_Alloc_Malloc(alloc, sizeof(*writer));
  if (!writer) {
    // TODO: Print Error
    return nullptr;
  }

  writer->alloc = alloc;

  return writer;
}

void
LISHP_Writer_Destroy(LISHP_Writer* writer)
{
  if (!writer) return;
  LISHP_Alloc_Free(writer->alloc, writer);
}
