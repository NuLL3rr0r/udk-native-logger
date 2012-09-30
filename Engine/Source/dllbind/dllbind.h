#ifndef __DLLBIND_H__
#define __DLLBIND_H__

#include "stdafx.h"

extern "C"
{	
	typedef void* (*ReallocFunctionPtrType)( void* Original, DWORD Count, DWORD Alignment );

	struct FDLLBindInitData
	{
		INT Version;
		ReallocFunctionPtrType ReallocFunctionPtr;
	};

	extern ReallocFunctionPtrType ReallocFunctionPtr;

	__declspec(dllexport) void DLLBindInit( FDLLBindInitData* InitData );
}

#endif
