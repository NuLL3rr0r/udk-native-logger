#ifndef LOG_HPP
#define LOG_HPP


#include <sstream>
#include <boost/function.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/scoped_ptr.hpp>

class Log
{
    friend class StaticStuff;

private:
    typedef boost::scoped_ptr<StaticStuff> StaticStuffPtr;

protected:
    typedef boost::function<void(const std::string &)> OnLogSentFunc;

public:
    enum EType {
        EType_Debug,
        EType_Notice,
        EType_Warn,
        EType_Error,
        EType_Fatal
    };

private:
    static const size_t PORT;

public:
    static OnLogSentFunc OnLogSent;

private:
    static StaticStuffPtr m_staticStuff;

private:
    boost::property_tree::ptree m_logTree;
    boost::property_tree::ptree m_msgTree;
    std::stringstream m_stream;

private:
    static void OnLogSentToServer(const std::string &log);

public:
    static void Initialize(const std::string &id);

public:
    Log(EType type, const std::string &file, const std::string &function, size_t line, ...);
    ~Log();

    template<typename ObjectType>
    Log &operator, (const ObjectType &v)
    {
        std::stringstream s;
        s << v;
        m_msgTree.push_back(std::make_pair("", s.str()));
        return *this;
    }
};

#define DBG(...)  \
    Log(Log::EType_Debug, __FILE__, __FUNCTION__, __LINE__), __VA_ARGS__;

#define NOTICE(...)  \
    Log(Log::EType_Notice, __FILE__, __FUNCTION__, __LINE__), __VA_ARGS__;

#define WARN(...)  \
    Log(Log::EType_Warn, __FILE__, __FUNCTION__, __LINE__), __VA_ARGS__;

#define ERR(...)  \
    Log(Log::EType_Error, __FILE__, __FUNCTION__, __LINE__), __VA_ARGS__;

#define FATAL(...)  \
    Log(Log::EType_Fatal, __FILE__, __FUNCTION__, __LINE__), __VA_ARGS__;


#endif /* LOG_HPP */

