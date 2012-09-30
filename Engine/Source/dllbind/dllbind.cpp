#include <assert.h>
#include <stdlib.h>

#include "udk_types.h"
#include "dllbind.h"

ReallocFunctionPtrType ReallocFunctionPtr = NULL;

void DLLBindInit( FDLLBindInitData* InitData )
{
	ReallocFunctionPtr = InitData->ReallocFunctionPtr;
}
