#ifndef LISHP_PRELUDE_H
#define LISHP_PRELUDE_H

#include <stdint.h>

/*****************************************************************************/
/*                                 INTERFACES                                */
/*****************************************************************************/

typedef struct LISHP_Runtime LISHP_Runtime;
typedef struct LISHP_Context LISHP_Context;

typedef uintptr_t          LISHP_Value;
typedef union LISHP_Object LISHP_Object;
typedef struct LISHP_Type  LISHP_Type;

typedef struct LISHP_Allocator        LISHP_Allocator;
typedef struct LISHP_Diagnostics      LISHP_Diagnostics;
typedef struct LISHP_GarbageCollector LISHP_GarbageCollector;
typedef struct LISHP_VirtualMachine   LISHP_VirtualMachine;
typedef struct LISHP_SymbolTable      LISHP_SymbolTable;
typedef struct LISHP_ConstantPool     LISHP_ConstantPool;
typedef struct LISHP_Reader           LISHP_Reader;
typedef struct LISHP_Writer           LISHP_Writer;

#endif /* LISHP_PRELUDE_H */
