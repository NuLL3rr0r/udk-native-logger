#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "log.hpp"

const std::string Log::LOG_PATH = "Logs";

std::string Log::m_logFile;

void Log::Initialize(const std::string &logFile)
{
    if (boost::filesystem::exists(LOG_PATH)) {
        if (!boost::filesystem::is_directory(LOG_PATH)) {
            boost::filesystem::remove(LOG_PATH);
        }
    }

    if (!boost::filesystem::exists(LOG_PATH)) {
        boost::filesystem::create_directories(LOG_PATH);
    }

    m_logFile = (boost::format("%1%\\%2%_%3%.txt")
                 % LOG_PATH
                 % logFile
                 % boost::algorithm::replace_all_copy(
                     boost::algorithm::replace_all_copy(
                         boost::posix_time::to_simple_string(
                             boost::posix_time::second_clock::local_time()),
                         ":", "-"),
                     " ", "_")
                 ).str();
}

Log::Log(const std::string &formattedLog)
{
    if(!m_stream.is_open()) {
        m_stream.open(m_logFile, std::ios_base::out | std::ios_base::app);
        m_stream.imbue(std::locale(m_stream.getloc(), new boost::posix_time::time_facet()));
    }

    m_stream << formattedLog;
}

Log::~Log()
{
    m_stream << std::endl;
    m_stream.flush();
    m_stream.close();
}
