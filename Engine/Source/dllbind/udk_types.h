#ifndef __UDK_TYPES_H__
#define __UDK_TYPES_H__

#include <windows.h>
#include <string>
#include <assert.h>
#include "dllbind.h"

#define UDK_DLLEXPORT __declspec(dllexport)

#define UBOOL unsigned __int32
#define UINT __int32

struct FVector
{
	float x,y,z;
};

struct FString
{
	wchar_t *Data;
	int ArrayNum;
	int ArrayMax;

	void UpdateArrayNum();

	int Num();
	void Reallocate(int newNum, bool bCompact = false);
	FString *Clone() const;
	void Copy(const FString &other);
	const wchar_t *GetText() const;
	void SetText(const wchar_t *text);

	FString &operator = (const wchar_t *text);
	FString &operator = (const char *text);
	FString &operator = (const std::wstring &text);
	FString &operator = (const std::string &text);
};

template<typename DataType>
struct TArray
{
	DataType* Data;

	int Capacity() const 
	{
		return ArrayNum;
	}

	int Num() const
	{
		return ArrayNum;
	}

	void Reallocate( int NewNum, bool bCompact=false )
	{
		ArrayNum = NewNum;
		if( ArrayNum > ArrayMax || bCompact )
		{			
			ArrayMax = ArrayNum;
			Data = (DataType*)(*ReallocFunctionPtr)( Data, ArrayMax * sizeof(DataType), 4 );
		}
	}
	
	void AddItem( DataType* const pNewItem )
	{
		Reallocate( ArrayNum + 1 );
		Data[ArrayNum - 1] = *pNewItem;
	}
	
	void AddItem( const DataType* pNewItem )
	{
		Reallocate( ArrayNum + 1 );
		Data[ArrayNum - 1] = *pNewItem;
	}
	
	void Empty( void )
	{
		Reallocate( 0 );
	}

	const DataType& operator [] ( unsigned index ) const
	{
		assert( index >= 0 && index < ArrayMax );

		return Data[index];
	}

	// BEOKEN
	static DataType* CloneItem( DataType* pItem )
	{		
		return pItem;
		DataType* pCopyInstance = (DataType*)(*ReallocFunctionPtr)( pItem, sizeof(DataType), 4 );
		std::memmove( pCopyInstance, pItem, sizeof(DataType) );
		return pCopyInstance;
	}

	// BROKEN
	void AddItems( const TArray<DataType>& source )
	{
		for( int i = 0; i < source.Num(); ++i )
		{
			AddItem( CloneItem( &source.Data[i] ) );
		}
	}
	
private:
	int ArrayNum;
	int ArrayMax;
};

#endif
