#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <queue>
#include <string>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <zmq.hpp>

namespace ULogger {
    class Client;
}

class ULogger::Client
{
private:
    typedef boost::shared_ptr<boost::thread> ThreadPtr;
    typedef boost::shared_ptr<zmq::context_t> ContextPtr;
    typedef boost::shared_ptr<zmq::socket_t> SocketPtr;

private:
    static const std::string MSG_RECEIVED;

public:
    boost::function<void(const std::string &)> OnMessageSent;

private:
    std::string m_id;
    bool m_running;
    size_t m_port;

    std::queue<std::string> m_requests;

    ThreadPtr m_workerThread;
    boost::mutex m_workerMutex;
    boost::mutex m_dataMutex;

    ContextPtr m_context;
    SocketPtr m_socket;

public:
    Client();
    Client(const std::string &id, size_t port);
    ~Client();

public:
    void SetId(const std::string &id);
    void SetPort(const size_t port);

    bool IsRunning();
    void Start();
    void Stop();

    void SendMessage(const std::string &msg);

private:
    std::string GetMessage(const zmq::message_t &msg);
    void SendRequest(const std::string &req);
    void SendRequests();
};


#endif /* CLIENT_HPP */

