#include "single_server.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../sentry/sentry_manager.hpp"
#include "../sentry/sentry_msg_handle.hpp"
#include "../../utils/time_helper.hpp"
#include "../../utils/time_manager.cpp"
#include "../../utils/log.hpp"

USING_NS_UTILS
USING_NS_SERVICE_CORE
USING_NS_PROTOCPP

bool SingleServer::onInitStart() {
    if (!onLoadConfig()) {
        log_error("load config failed");
        return false;
    }
    if (!onInitNetWork()) {
        log_error("init network failed");
        return false;
    }
    if (!onInitLogic()) {
        log_error("init logic failed");
        return false;
    }
    return true;
}

void SingleServer::onRunServer() {
    log_trac("[ %s start at %s ]", getServerName(), TimeHelper::getNowTimeStr().c_str());
    setbuf(stdout, NULL);
}

bool SingleServer::onCloseServer() {
    closeLogic();
    closeNetwork();
    return true;
}

bool SingleServer::closeLogic() {
    ObjectManager::close();
    TimeManager::cancel();
    return true;
}

bool SingleServer::onLoadConfig() {
    if (!onLoadXmlConfig()) {
        return false;
    }
    retrun true;
}

bool SingleServer::onInitNetWork() {
    //fixed thread 1
    startNetwork(1);

    if(!listenAt("0.0.0.0",23333 , m_sentryListner)
       return false;
    }

    SentryMsgHandler::initMsgHandler();
    return true;
}

bool SingleServer::onInitLogic() {
    CmdManager::init();
    TimeManager::init(getIos());

    ObjectManager::init();
    return true;
}

bool SingleServer::onLoadXmlConfig(){
    return true;
}

void SingleServer::onAddSentry(const TcpSocketPtr& s) {
    ObjectManager::addConnector(s);
}

void SingleServer::onDelSentry(const TcpSocketPtr& s) {
    ObjectManager::delConnector(s);
}

void SingleServer::onSentryMsg(const TcpSocket& s, uint16 cmd, const MsgBufPtr& msg) {
    SentryHandler::s_msgHandler.msgHandler(s,cmd,msg,m_sentryListner.name());
}
