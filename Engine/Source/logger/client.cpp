#include <cassert>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "client.hpp"

using namespace ULogger;

const std::string Client::MSG_RECEIVED = "RCVD!";

Client::Client() :
    m_running(false),
    m_port(0)
{
}

Client::Client(const std::string &id, size_t port) :
    m_id(id),
	m_running(false),
    m_port(port)
{
}

Client::~Client()
{
    if (m_running) {
        Stop();
    }
}

void Client::SetId(const std::string &id)
{
    if (m_running)
        return;

    m_workerMutex.lock();
    m_id = id;
    m_workerMutex.unlock();
}

void Client::SetPort(const size_t port)
{
    if (m_running)
        return;

    m_workerMutex.lock();
    m_port = port;
    m_workerMutex.unlock();
}

bool Client::IsRunning()
{
    return m_running;
}

void Client::Start()
{
    m_workerMutex.lock();

    assert(boost::algorithm::trim_copy(m_id) != "");
    assert(m_port != 0);

    if (m_running) {
        m_workerMutex.unlock();
        return;
    }

    std::string tcpURL((boost::format("tcp://localhost:%1%") % boost::lexical_cast<std::string>(m_port)).str());

    m_context = ContextPtr(new zmq::context_t(1));
    m_socket = SocketPtr(new zmq::socket_t(*m_context.get(), ZMQ_REQ));

    int linger = 0;
    m_socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

    m_socket->connect(tcpURL.c_str());

    m_running = true;

    m_workerThread = ThreadPtr(new boost::thread(&Client::SendRequests, this));
    m_workerThread->detach();

    m_workerMutex.unlock();
}

void Client::Stop()
{
    m_workerMutex.lock();

    if (!m_running) {
        m_workerMutex.unlock();
        return;
    }

    m_running = false;
    m_workerThread->interrupt();

    std::string tcpURL((boost::format("tcp://localhost:%1%") % boost::lexical_cast<std::string>(m_port)).str());
    zmq_disconnect((void *)m_socket.get(), tcpURL.c_str());

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

void Client::SendMessage(const std::string &msg)
{
    if (!m_running)
		return;

    m_dataMutex.lock();
    m_requests.push(msg);
    m_dataMutex.unlock();
}

std::string Client::GetMessage(const zmq::message_t &msg)
{
    return std::string(static_cast<const char *>(msg.data()), msg.size());
}

void Client::SendRequest(const std::string &req)
{
    boost::property_tree::ptree reqTree;
    reqTree.put("req.clientId", m_id);
    reqTree.put("req.message", req);

    std::stringstream reqStream;
    boost::property_tree::write_json(reqStream, reqTree);
    std::string reqJSON(reqStream.str());

    zmq::message_t request(reqJSON.size());
    memcpy(request.data(), reqJSON.data(), reqJSON.size());

    bool rc = false;
    do {
        boost::this_thread::disable_interruption di;
        m_workerMutex.lock();
        try {
            rc = m_socket->send(request, ZMQ_NOBLOCK);
        } catch (...){
        }
        m_workerMutex.unlock();

        if (rc) {
            zmq::message_t response;
            do {
                m_workerMutex.lock();
                try {
                    rc = m_socket->recv(&response, ZMQ_NOBLOCK);
                } catch (...){
                }
                m_workerMutex.unlock();

                if (rc) {
                    if (GetMessage(response) == Client::MSG_RECEIVED) {
                        if (!OnMessageSent.empty()) {
                            OnMessageSent(req);
                        }
                    }
                }
                boost::this_thread::restore_interruption ri(di);
                boost::this_thread::interruption_point();
            } while (!rc);
        }

        boost::this_thread::restore_interruption ri(di);
        boost::this_thread::interruption_point();
    } while (!rc);
}

void Client::SendRequests()
{
    while(m_running) {
        if (m_requests.size() > 0) {
            m_dataMutex.lock();
            std::string messageStr(m_requests.front());
            m_requests.pop();
            m_dataMutex.unlock();
            SendRequest(messageStr);
        }
        boost::this_thread::interruption_point();
    }
}

