#ifndef SERVER_HPP
#define SERVER_HPP


#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <zmq.hpp>

namespace ULogger {
    class Server;
}

class ULogger::Server
{
private:
    typedef boost::shared_ptr<boost::thread> ThreadPtr;
    typedef boost::shared_ptr<zmq::context_t> ContextPtr;
    typedef boost::shared_ptr<zmq::socket_t> SocketPtr;

private:
    static const std::string MSG_RECEIVED;
    static const std::string MSG_INVALID;

public:
    boost::function<void(const std::string &, const std::string &)> OnMessageReceived;

private:
	bool m_running;
	size_t m_port;

    ThreadPtr m_workerThread;
    boost::mutex m_workerMutex;

    ContextPtr m_context;
    SocketPtr m_socket;

public:
    Server();
    Server(size_t port);
    ~Server();

public:
    void SetPort(const size_t port);

    bool IsRunning();
	void Start();
	void Stop();

private:
	void Listen();
	std::string GetMessage(const zmq::message_t &msg);
    bool SendResponse(const std::string &str);
};


#endif /* SERVER_HPP */

