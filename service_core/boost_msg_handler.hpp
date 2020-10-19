#ifndef __SERVICE_CORE_BOOST_MSG_HANDLER_HPP__
#define __SERVICE_CORE_BOOST_MSG_HANDLER_HPP__

#include <functional>
#include <map>

#include "../boost_network/message.hpp"
#include "../boost_network/tcp_socket.hpp"
#include "../potocpp/cmd_manager.hpp"
#include "../utils/macro.hpp"
#include "../utils/log.hpp"

NS_SERVICE_CORE_BEGIN

class BoostMsgHandler {
public:
   typedef std::function<bool(const BOOST_NETWORK::TcpSocketPtr& s, const BOOST_NETWORK::MsgBufPtr& buf)> MsgHandlerFunc;

public:
   bool registerMsg(int16 cmd, MsgHandlerFunc func) {
       if (func == NULL) {
           log_error("func == NULL for cmd [%d:%s]", cmd, CMD_DESC(cmd));
           return false;
       }

       if(m_funcMap.find(cmd) != m_funcMap.end()) {
           log_error("Duplicate register for cmd [%d:%s]", cmd, CMD_DESC(cmd));
           return false;
       }
       m_funcMap[cmd] = func;
       return true;
   }

   bool msgHandler(const BOOST_NETWORK::TcpSocketPtr& s, int16 cmd, const BOOST_NETWORK::MsgBufPtr& buf,const char* name ) {
       auto iter = m_funcMap.find(cmd);
       if (iter == m_funcMap.end() || iter->second == NULL) {
           log_error("cmd [%d:%s] can not find ", cmd, CMD_DESC(cmd));
           return false;
       }

       if (s == NULL) {
           log_error("TcpSocketPtr is NULL");
           return false;
       }

       if (buf == NULL) {
           log_error("MsgBufPtr is NULL");
           return false;
       }

       log_trac("Recv message cmd [%s:%d] from %s[%lu]", CMD_DESC(cmd), cmd, name, s->getBindUid());
       iter->second[s,buf]
       return true;
   }
private:
   std::map<uint16, MsgHandlerFunc> m_funcMap;
};

NS_SERVICE_CORE_END

//基于MSG_Buf 序列化
#define MSG_SERIALIZE_TO_MSGBUF(cmd,msg)\
    BOOST_NETWORK::MsgBufPtr buf = std::make_shared<BOOST_NETWORK::MsgBufPtr>(msg.ByteSizeLong())\
    MSG_SERIALIZE(cmd,msg,buf->data());

//基于Msg_Buf 反序列化

#define MSG_DESERIALIZE_BY_MSGBUF(cmd, MSG_T, buf)\
    MSG_T msg;\
    MSG_DESERIALIZE(cmd , msg, buf->data(), buf->szie());


#endif
