#include "lishp/lishp_object.h"
#include "lishp/lishp_garbage_collector.h"

#include <assert.h>
#include <string.h>

/*****************************************************************************/
/*                                 LIFE CYCLE                                */
/*****************************************************************************/

[[nodiscard]] LISHP_Object*
LISHP_Object_InitializeNil(LISHP_Object* obj)
{
  assert(obj);
  assert(LISHP_Object_ToHeader(obj)->object_size == sizeof(LISHP_Object_Nil));

  obj->as_nil.tag = LISHP_OBJECT_TAG_NIL;

  return obj;
}

[[nodiscard]] LISHP_Object*
LISHP_Object_InitializeCons(LISHP_Object* obj, LISHP_Value car, LISHP_Value cdr)
{
  assert(obj);
  assert(LISHP_Object_ToHeader(obj)->object_size == sizeof(LISHP_Object_Cons));

  obj->as_cons.tag = LISHP_OBJECT_TAG_CONS;
  obj->as_cons.car = car;
  obj->as_cons.cdr = cdr;

  return obj;
}

[[nodiscard]] LISHP_Object*
LISHP_Object_InitializeStringlike(LISHP_Object*                   obj,
                                  struct LISHP_Object_Stringlike* str,
                                  LISHP_Object_Tag                tag,
                                  const char*                     data,
                                  size_t                          len)
{
  assert(obj);
  assert(str);
  assert(LISHP_Object_ToHeader(obj)->object_size ==
         sizeof(struct LISHP_Object_Stringlike) + (len + 1) * sizeof(*data));

  str->tag    = tag;
  str->hash   = 0; // TODO
  str->length = len;

  memcpy(str->data, data, len);
  str->data[len] = '\0';

  return obj;
}

[[nodiscard]] LISHP_Object*
LISHP_Object_InitializeFunctionlike(LISHP_Object*                     obj,
                                    struct LISHP_Object_Functionlike* func,
                                    LISHP_Object_Tag                  tag,
                                    LISHP_Value                       params,
                                    LISHP_Value                       body,
                                    LISHP_Value                       env)
{
  assert(obj);
  assert(func);
  assert(LISHP_Object_ToHeader(obj)->object_size ==
         sizeof(struct LISHP_Object_Functionlike));

  func->tag    = tag;
  func->params = params;
  func->body   = body;
  func->env    = env;

  return obj;
}

void
LISHP_Object_Finalize(LISHP_Object*                     obj,
                      [[maybe_unused]] LISHP_Allocator* alloc)
{
  assert(obj);

  switch (LISHP_Object_GetTag(obj)) {
      // All of these have no internal resources, so no finalize needed
    case LISHP_OBJECT_TAG_NIL     : break;
    case LISHP_OBJECT_TAG_CONS    : break;
    case LISHP_OBJECT_TAG_SYMBOL  : break;
    case LISHP_OBJECT_TAG_KEYWORD : break;
    case LISHP_OBJECT_TAG_FUNCTION: break;
    case LISHP_OBJECT_TAG_MACRO   : break;
  }
}

/*****************************************************************************/
/*                                  VISITORS                                 */
/*****************************************************************************/

void
LISHP_Object_Visit(LISHP_Object* obj,
                   bool          (*visitor)(LISHP_Object*, void*),
                   void*         ctx)
{
  assert(obj);
  assert(visitor);

  auto continue_to_recurse = visitor(obj, ctx);
  if (!continue_to_recurse) return;

  switch (LISHP_Object_GetTag(obj)) {

    // Atomic values, no recursion needed
    case LISHP_OBJECT_TAG_NIL     : break;
    case LISHP_OBJECT_TAG_SYMBOL  : break;
    case LISHP_OBJECT_TAG_KEYWORD : break;
    case LISHP_OBJECT_TAG_FUNCTION: break;
    case LISHP_OBJECT_TAG_MACRO   : break;

    case LISHP_OBJECT_TAG_CONS: {
      // TODO: Recurse
      break;
    }
  }
}
