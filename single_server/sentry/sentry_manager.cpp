#include "sentry_manager.hpp"

#include "utils/log.hpp"
#include "protocpp/op_cmd.hpp"
#include "protocpp/protos/servers.bp.h"

USING_UTILS_NS

uint64 ObjectManager::s_globalUid = 0;
UTILS::TimerPtr ObjectManager::s_timer;

std::<uint64, ObjectPtr> ObjectManager::s_objectMap;

void Object::checkActive(int32 now) {
    if (m_activeTime + LINK_ACTIVE_TIME > now ) {
        proto::server::MsgPing request;
        global::pServer->sendMsgBySocket(m_socketPtr, PROTOCPP::MsgPing, request);
    }
}

void Object::testLimitTcp(uint32 times) {
    do {
        times --;
        proto::server::MsgTestTcpReply reply;
        global::pServer->sendMsgBySocket(m_socketPtr, PROTOCPP::MsgTestTcpReply, reply);
    } while(times > 0);
    global::pServer->onClose(m_socketPtr);
    ObjectManager->deConnector(m_sockerPtr);
}

void ObjectManager::addConnector(const BOOST_NETWORK::TcpSocketPtr& s) {
    if (s == NULL) {
        log_error("TcpSocketPtr == NULL");
        return;
    }
    ObjectPtr ob = std::make_shared<Object>(s, getIdleUid());
    if (addObject(ob)) {
        log_Info(" Add object [%lu] succeed at socket[%d][%s:%u]", ob->getUid(), s->getSocketId(), s->getIp().c_str(), s->getPort());
    } else {
        log_error("Add object [%lu] failed at socket[%d][%s:%u]", ob->getUid(), s->getSocketId(), s->getIp().c_str(), s->getPort());
    }
}

void ObjectManager::delConnector(const BOOST_NETWORK::TcpSocketPtr& s) {
    if (s == NULL) {
        log_error("TcpSocketPtr == NULL");
        return;
    }
    if (delObject(s->getBindUid())) {
        log_Info("Del object [%lu] succeed at socket[%d][%s:%u]", s->getBindUid(), s->getSocketId(), s->getIp().c_str(), s->getPort());
    } else {
        log_error("Del object [%lu] failed at socket[%d][%s:%u]", s->getBindUid(), s->getSocketId(), s->getIp().c_str(), s->getPort());
    }
}

bool ObjectManager::onInit() {
    s_timer.reset(new UTILS::TimeBase(1));
    s_timer.afterFunc(5, ObjectManager::onTimer());
    return true;
}

void ObjectManager::onClose() {
    s_timer.cancel();
}

bool ObjectManager::addObject(const objectPtr& ob) {
    if (NULL == findObject(ob->getUid())) {
        return false;
    }
    s_objectMap.insert(std::make_pair(ob->getUid, ob));
    return true;
}

bool ObjectManager::delObject(uint64 uid) {
    if (NULL == findObject(uid)) {
        return false;
    }
    s_objectMap.erase(uid);
    return true;
}

ObjectPtr ObjectManager::findObject(uint64 uid) {
    auto iter = s_objectMap.find(uid);
    if (iter == s_objectMap.end() || iter->second == NULL) {
        return NULL
    }
    return iter->second;
}

uint64 getIdleUid() {
    return s_globalUid++;
}

void ObjectManager::onTimer() {
    int32 now = TimeHelper::getNowTimeStr();

    for(auto iter = s_objectMap.begin(); iter != s_objectMap.end();) {
        if (iter->second == NULL) {
            iter++;
            continue;
        }
        ObjectPtr ob = iter->second;
        if (ob->isTimeOut(now)) {
            global::pServer->postCloseSocket(ob->getSocketPtr());
            iter = s_objectMap.erase(ob);
            continue;
        }
        ob->checkActive(now);
        iter++;
    }
    s_timer.afterFunc(5, ObjectManager::onTimer());
}
