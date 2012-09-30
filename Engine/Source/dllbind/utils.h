#ifndef __UTILS_H__
#define __UTILS_H__

#include "stdafx.h"
#include <string>

#include <string> 
#include <vector>
#include "udk_types.h"

std::string wstrtostr(const std::wstring &wstr);
std::wstring strtowstr(const std::string &str);

std::vector<std::wstring> SplitW( const std::wstring& text );
std::vector<std::string> Split( const std::string& text );

// Helper method to fill an unreal list of strings with an stl vector of strings.
void PopulateListOfStrings( std::vector<std::wstring> stl_list, TArray<FString>& out_unrealList );

bool ParseListOfStrings( const wchar_t* const text, TArray<FString>& out_list );

UBOOL ParseBool( std::string text );

#endif
