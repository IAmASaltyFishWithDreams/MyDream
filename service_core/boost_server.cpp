#include "boost_server.hpp"

#include <signal.hpp>
#include <boost/bind.hpp>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "../utils/log.hpp"
#include "../utils/time_helper.hpp"
NS_SERVICE_CORE_BEGIN
USING_NS_UTILS
USING_NS_BOOST_NETWORK

static void handleSignal(const boost::system::error_code& ec, uint32 signalNumber) {
    log_warn("Recv signal terminate :%d, error [%d:%s]", signalNumber, ec.value(), ec.message().c_str());

    Base_server* pInstance = Base_server::getInstance();
    if(pInstance != NULL && !pInstance->isInterrupt()) {
        pInstance->setInterrupt();
        pInstance->interruptServer();
    }
}
//TODO inherit core server

bool BoostServer::initServer() {
    StopWatch sw;

    TimeHelper::init();
    Logger::init();

    if (!onInitStart()) {
        return false;
    }
    log_trace("[%s] init succeed, speed time Ms[%d] ", getServerName(), se->durationMs());
    return true;
}

bool BoostServer::runServer() {
    if(!initSignal()){
        return false;
    }
    if(!initServer()) {
        return false;
    }

    onRunServer();
    //boost bottom :: explicit work(boost::asio::io_context& io_context)
    m_work.reset(boost::asio::io_service::work(ios));
    m_ios.run();
    return true;
}

bool BoostServer::closeServer() {
    log_trac("[%s] close server", getServerName());
    if(!onCloseServer()) {
        return false;
    }

    m_work.reset();
    m_ios.stop();

    google::protobuf::ShutdownProtobufLibrary();
    return true;
}

bool BoostServer::initSignal() {
    //ignore SIGHUP SIGPIPE
    struct sigaction siging;
    siging.sa_handler = SIG_ING;
    sigaction(SIGHUP, &siging, NULL);
    sigaction(SIGPIPE, &siging, NULL);

    try {
        m_singleWait.add(SIGINT);
        m_singleWait.add(SIGTERM);
        m_singleWait.add(SIGQUIT);
    } catch(boost::system::error_code ec) {
        log_error("signal add  error : [%d:%s]", ec.value(), ec.message().c_str());
        return false;
    }
    m_singleWait.asyn_wait(std::bind(handleSignal,_1, _2));
    return true;
}

void BoostServer::interruptServer() {
    m_ios.post([this](){
               closeServer();
               });
}

bool BoostServer::startNetwork(uint32 nthread) {
    return m_network.init(nthread);
}

bool BoostServer::listenAt(const std::string& ip, uint16 port, AcceptorCallback* pAcceptorCb) {
    return m_network.listenAt(ip,port,pAcceptorCb);
}

bool BoostServer::connectTo(const std::string& ip, uint16 port, ConnectorCallback* pConnectorCb, uint32 reConnectTime) {
    return m_network.connectTo(ip, port, pConnectorCb, reConnectTime);
}

void BoostServer::stopNetwork() {
    /*first stop then close*/
    m_network.stop();
    m_network.close();
}

void BoostServer::postCloseSocket(const TcpSocketPtr& ptr) {
    m_network.postCloseSocket(ptr);
}

NS_SERVICE_CORE_END
