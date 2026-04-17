#include "lishp/lishp_read.h"
#include "lishp/lishp_alloc.h"

#include <assert.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Reader*
LISHP_Reader_Create(LISHP_Alloc* alloc, LISHP_GC* gc)
{
  assert(alloc);

  LISHP_Reader* reader = LISHP_Alloc_Malloc(alloc, sizeof(*reader));
  if (!reader) {
    // TODO: Print Error
    return nullptr;
  }

  reader->alloc = alloc;
  reader->gc    = gc;

  return reader;
}

void
LISHP_Reader_Destroy(LISHP_Reader* reader)
{
  if (!reader) return;
  LISHP_Alloc_Free(reader->alloc, reader);
}
