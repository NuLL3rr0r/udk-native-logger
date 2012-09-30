#include <cassert>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "log.hpp"
#include "client.hpp"

class StaticStuff {
private:
    typedef boost::scoped_ptr<ULogger::Client> ClientPtr;

public:
    bool IsInitialized;
    std::map<Log::EType, std::string> TypeStrings;
    ClientPtr Client;
    std::string Id;

public:
    StaticStuff()
        : IsInitialized(false),
          Client(new ULogger::Client())
    {
        TypeStrings.clear();
        TypeStrings[Log::EType_Debug] = "Debug";
        TypeStrings[Log::EType_Notice] = "Notice";
        TypeStrings[Log::EType_Warn] = "Warn";
        TypeStrings[Log::EType_Error] = "Error";
        TypeStrings[Log::EType_Fatal] = "Fatal";
    }

    ~StaticStuff()
    {
        if (Client->IsRunning()) {
            Client->Stop();
        }
        Client.reset();
    }
};

const size_t Log::PORT = 1317;
Log::OnLogSentFunc Log::OnLogSent;
Log::StaticStuffPtr Log::m_staticStuff(new StaticStuff());

void Log::OnLogSentToServer(const std::string &log)
{
    if (!OnLogSent.empty()) {
        OnLogSent(log);
    }
}

void Log::Initialize(const std::string &id)
{
    if (m_staticStuff->IsInitialized)
        return;

    m_staticStuff->Id = id;

    if (m_staticStuff->Client->IsRunning())
        m_staticStuff->Client->Stop();

    m_staticStuff->Client->SetId(id);
    m_staticStuff->Client->SetPort(PORT);
    m_staticStuff->Client->OnMessageSent = boost::bind(&Log::OnLogSentToServer, _1);
    m_staticStuff->Client->Start();

    m_staticStuff->IsInitialized = true;
}

Log::Log(EType type, const std::string &file, const std::string &function, size_t line, ...)
{
    assert(m_staticStuff->IsInitialized);

    m_stream.clear();
    m_stream.imbue(std::locale(m_stream.getloc(), new boost::posix_time::time_facet()));

    m_logTree.put("log.type", m_staticStuff->TypeStrings[type]);
    m_logTree.put("log.timestamp", boost::posix_time::second_clock::local_time());
    m_logTree.put("log.file", file);
    m_logTree.put("log.function", function);
    m_logTree.put("log.line", line);
}

Log::~Log()
{
    m_logTree.get_child("log").push_back(std::make_pair("messages", m_msgTree));
    boost::property_tree::write_json(m_stream, m_logTree);

    m_staticStuff->Client->SendMessage(m_stream.str());
}

