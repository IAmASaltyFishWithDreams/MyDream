#ifndef __SINGLE_SERVER_SENTRY_MANAGER_HPP___
#define __SINGLE_SERVER_SENTRY_MANAGER_HPP___

#include <map>
#include <memory>

#include "../../boost_network/tcp_socket.hpp"
#include "../main/server_global.hpp"
#include "../main/single_server.hpp"
#include "../../utils/log.hpp"
#include "../../utils/time_helper.hpp"
#include "../../utils/time_manager.hpp"

class Object {
public:
    Object(const BOOST_NETWORK::TcpSocketPtr& s, uint64 uid) : m_socketPtr( s ), m_uid( uid ) {
        s->bindUid(uid);
        m_activeTime = UTILS::TimeHelper::getCurrentSecond();
    }

public:
    BOOST_NETWORK::TcpSocketPtr getSocketPtr() { return m_socketPtr; }
    uint64 getUid() { return m_uid; }

    void checkActive(int32 now);
    bool isTimeOut(int32 now) { return  m_activeTime + LINL_OUTTIME > now; }
    void setActive() { m_activeTime = UTILS::TimeHelper::getCurrentSecond(); }

public:
    void testLimitTcp(uint32 times);

private:
    BOOST_NETWORK::TcpSocketPtr m_socketPtr; //socket
    uint64 m_uid;                            //uid
    int32 m_activeTime;                      // active Time

};

typedef std::shared_ptr<Object> ObjectPtr;

class ObgectManager {
public:
    static void addConnector(const BOOST_NETWORK::TcpSocketPtr& s);

    static void delConnector(const BOOST_NETWORK::TcpSocketPtr& s);

public:
    static bool onInit();

    static void onClose();

    static bool addObject(const ObjectPtr& ob);

    static bool delObject(uint64 uid);

    static ObjectPtr findObject(uint64 uid);

    static uint64 getIdleUid();

public:
    static void onTimer();

pirvate:
    static std::map<uint64, ObjectPtr> s_objectMap;
    static uint64 s_globalUid;
    static UTILS::TimerPtr s_timer;
};
#endif
