#include "utils.h"
#include <string> 
#include <sstream> 
#include <algorithm> 
#include <iterator> 

std::string wstrtostr(const std::wstring &wstr)
{
    // Convert a Unicode string to an ASCII string
    std::string strTo;
    char *szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

std::wstring strtowstr(const std::string &str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t *wszTo = new wchar_t[str.length() + 1];
    wszTo[str.size()] = L'\0';
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
}

std::vector<std::wstring> SplitW( const std::wstring& text )
{	
	std::vector<std::wstring> output;

	typedef std::vector<std::string>::iterator ListIte;
	std::vector<std::string> splitList = Split( wstrtostr(text) );

	for( ListIte currentItem = splitList.begin(); currentItem != splitList.end(); ++currentItem )
	{
		output.push_back( strtowstr( *currentItem ) );
	}

	return output;
}

std::vector<std::string> Split( const std::string& text )
{	
	std::vector<std::string> tokens;
	std::istringstream iss(text);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter<std::vector<std::string> >(tokens));

	return tokens;
}

bool ParseListOfStrings( const wchar_t* const text, TArray<FString>& out_list )
{
	if( NULL == text )
	{
		return false;
	}

	std::vector<std::wstring> splitList = SplitW( text );
	PopulateListOfStrings( splitList, out_list );

	return true;
}

void PopulateListOfStrings( std::vector<std::wstring> stl_list, TArray<FString>& out_unrealList )
{
	std::vector<std::wstring>::iterator stlNode = stl_list.begin();
	for( ; stlNode != stl_list.end() ; ++stlNode )
	{
		FString* pNewString = new FString();
		std::wstring elementText = (*stlNode);
		
		pNewString->SetText( elementText.c_str() );

		out_unrealList.AddItem( pNewString );
	}
}

UBOOL ParseBool( std::string text )
{
	if( text == std::string("true") || text == std::string("yes") )
	{
		return TRUE;
	}

	return FALSE;
}