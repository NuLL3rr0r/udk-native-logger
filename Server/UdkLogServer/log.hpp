#ifndef LOG_HPP
#define LOG_HPP


#include <fstream>
#include <string>

class Log
{
private:
    static const std::string LOG_PATH;

private:
    static std::string m_logFile;

private:
    std::ofstream m_stream;

public:
    static void Initialize(const std::string &logFile);

public:
    Log(const std::string &formattedLog);
    ~Log();
};

#define LOG(FORMATTEDLOG)  \
    Log(FORMATTEDLOG);


#endif /* LOG_HPP */

