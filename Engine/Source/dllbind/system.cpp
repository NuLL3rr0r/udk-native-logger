#include <shlobj.h>
#include <windows.h>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "system.hpp"

using namespace std;
using namespace boost;

string System::DirectorySeparatorChar()
{
	static string separator = "\\";
	return separator;
}

string System::GetCurrentPath()
{
	char pathBuffer[MAX_PATH];
    DWORD result = GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
	string path(pathBuffer);

	if (result != ERROR_SUCCESS) {
		string::size_type pos = path.find_last_of(DirectorySeparatorChar());
		path = path.substr(0, pos);
	} else {
		MessageBoxA(0, (format("Error code: %1") % result).str().c_str(), "IO Error", MB_OK);
	}

    return path;
}

string System::GetGameFolderPath(EGameFolder gameFolder)
{
	string gameRootPath(GetGameRootPath());

	if (!algorithm::ends_with(gameRootPath, DirectorySeparatorChar()))
		gameRootPath += DirectorySeparatorChar();

	string udkGameFolderPath(gameRootPath + "UDKGame");

	switch (gameFolder) {
	case EGameFolder_UDKGame:
		return udkGameFolderPath;

    case EGameFolder_Content:
		return udkGameFolderPath + DirectorySeparatorChar() + "Content";

    case EGameFolder_Logs:
		return udkGameFolderPath + DirectorySeparatorChar() + "Logs";

	case EGameFolder_Subtitles:
		return udkGameFolderPath + DirectorySeparatorChar() + "Content"
			+ DirectorySeparatorChar() + "Subtitles";

	default:
		return "";
	}
}

std::string System::GetGameRootPath()
{
	string path(GetCurrentPath());

	// We have to omit
	//	'Binaries\Win32\', 'Binaries\Win64\'
	// or
	//	'Binaries\Win32', 'Binaries\Win64'
	path = path.substr(0, path.rfind("Binaries" + DirectorySeparatorChar() + "Win") - 1);

	return path;
}

string System::GetSystemFolderPath(int specialFolder)
{
	char pathBuffer[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, specialFolder, NULL, SHGFP_TYPE_CURRENT, pathBuffer);
	string path(pathBuffer);

    if (result == S_OK) {
		if (algorithm::ends_with(path, DirectorySeparatorChar())) {
			path = path.substr(0, path.length() - 2);
		}
	} else {
		MessageBoxA(0, (format("Error code: %1") % result).str().c_str(), "IO Error", MB_OK);
	}

	return path;
}

