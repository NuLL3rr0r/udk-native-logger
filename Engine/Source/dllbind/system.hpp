#ifndef SYSTEM_HPP
#define SYSTEM_HPP


#include <string>

class System
{
public:
	enum EGameFolder {
        EGameFolder_UDKGame,
        EGameFolder_Content,
        EGameFolder_Logs,
        EGameFolder_Subtitles
    };

public:
	static std::string DirectorySeparatorChar();
	static std::string GetCurrentPath();
	static std::string GetGameFolderPath(EGameFolder gameFolder);
	static std::string GetGameRootPath();
	static std::string GetSystemFolderPath(int specialFolder);
};


#endif /* SYSTEM_HPP */

