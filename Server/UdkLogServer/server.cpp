#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "server.hpp"

using namespace ULogger;

const std::string Server::MSG_RECEIVED = "RCVD!";
const std::string Server::MSG_INVALID = "INVLD!";

Server::Server() :
    m_running(false),
    m_port(0)
{
}

Server::Server(size_t port) :
    m_running(false),
    m_port(port)
{
}

Server::~Server()
{
    if (m_running) {
        Stop();
    }
}

void Server::SetPort(const size_t port)
{
    if (m_running)
        return;

    m_workerMutex.lock();
    m_port = port;
    m_workerMutex.unlock();
}

bool Server::IsRunning()
{
    return m_running;
}

void Server::Start()
{
    m_workerMutex.lock();

    assert(m_port != 0);

    if (m_running) {
        m_workerMutex.unlock();
        return;
    }

    std::string tcpURL((boost::format("tcp://*:%1%") % boost::lexical_cast<std::string>(m_port)).str());

    try {
        m_context = ContextPtr(new zmq::context_t(1));
        m_socket = SocketPtr(new zmq::socket_t(*m_context.get(), ZMQ_REP));

        int linger = 0;
        m_socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

        m_socket->bind(tcpURL.c_str());
    }
    catch(zmq::error_t &ex) {
        throw std::exception((boost::format("Cannot listen on port '%1%': '%2%'")
                              % boost::lexical_cast<std::string>(m_port)
                              % ex.what()).str().c_str());
    }
    catch(...) {
        throw std::exception((boost::format("Cannot listen on port %1% !!")
                              % boost::lexical_cast<std::string>(m_port)).str().c_str());
    }

    m_running = true;

    m_workerThread = ThreadPtr(new boost::thread(&Server::Listen, this));
    m_workerThread->detach();

    m_workerMutex.unlock();
}

void Server::Stop()
{
    m_workerMutex.lock();

    if (!m_running) {
        m_workerMutex.unlock();
        return;
    }

    m_running = false;
    m_workerThread->interrupt();

    std::string tcpURL((boost::format("tcp://*:%1%") % boost::lexical_cast<std::string>(m_port)).str());
    zmq_unbind((void *)m_socket.get(), tcpURL.c_str());

    try {
        m_socket->close();
        m_context->close();
    } catch(...) {
    }

    m_workerThread.reset();
    m_socket.reset();
    m_context.reset();

    m_workerMutex.unlock();
}

void Server::Listen()
{
    bool rc;
    while(m_running) {
        boost::this_thread::disable_interruption di;

        zmq::message_t request;

        m_workerMutex.lock();
        try {
            rc = m_socket->recv(&request, ZMQ_NOBLOCK);
        } catch(...) {
            rc = false;
        }
        m_workerMutex.unlock();

        if (rc) {
            try {
                std::stringstream reqJSON;
                reqJSON << GetMessage(request);

                boost::property_tree::ptree reqTree;
                boost::property_tree::read_json(reqJSON, reqTree);

                if (!OnMessageReceived.empty()) {
                    OnMessageReceived(reqTree.get<std::string>("req.clientId"),
                                      reqTree.get<std::string>("req.message"));
                }

                SendResponse(MSG_RECEIVED);
            }

            catch (...) {
                SendResponse(MSG_INVALID);
            }
        }

        boost::this_thread::restore_interruption ri(di);
        boost::this_thread::interruption_point();
    }
}

std::string Server::GetMessage(const zmq::message_t &msg)
{
    return std::string(static_cast<const char *>(msg.data()), msg.size());
}

bool Server::SendResponse(const std::string &str)
{
    zmq::message_t msg(str.size());
    memcpy(msg.data(), str.data(), str.size());

    m_workerMutex.lock();
    bool rc;
    try {
        rc = m_socket->send(msg, ZMQ_NOBLOCK);
    } catch (...){
        rc = false;
    }
    m_workerMutex.unlock();

    return rc;
}


