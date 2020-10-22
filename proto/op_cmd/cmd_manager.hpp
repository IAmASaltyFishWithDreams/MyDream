#ifndef __PROTOCPP_CMD_MANAGER_HPP__
#define __PROTOCPP_CMD_MANAGRT_HPP__

#include <map>
#include <string>

#include "../../utils/macro.hpp"
#include "../../utils/log.hpp"

NS_PROTOCPP_BEGIN

class CmdManager {
public:
    static void init();

    static void clear();

    static const char* getMsgDesc(uint32 cmd);

    template <typename MSG_T>
    static void msgSerialize(uint16 cmd, const MSG_T& msg, char* pData);

    template <typename MSG_T>
    static void msgDeserialize(uint16 cmd, MSG_T& msg, const char* pData, uint16 size);

private:
    std::map<uint32, std::string> m_descMap;
};

template <typename MSG_T>
void CmdManager::msgSerialize(uint16 cmd, const MSG_T& msg, char* pData) {
    if(!pData) {
        log_error("pData == NULL, msg [%s:%d] serialize failed",getMsgDesc(cmd), cmd);
        return;
    }

    if(!msg.SerializeToArray(pData, msg.ByteSizeLong())) {
        log_error("Msg [%s:%d] serialize failed, ", getMsgDesc(cmd), cmd);
        return;
    }
}

template <typename MSG_T>
void CmdManager::msgDeserialize(uint16 cmd, MSG_T& msg, const char* pData, uint16 size) {
    if(!pData) {
        log_error("pData == NULL, msg [%s:%d] serialize failed", getMsgDesc(cmd), cmd);
        return;
    }
    if(!msg.ParseFromArray(pData,size)) {
        log_error("msg [%s:%d] serialize failed", getMsgDesc(cmd), cmd);
        return;
    }
}
NS_PROTOCPP_END
#define CMD_DESC(cmd)\
    PROTOCPP::CmdManager::getMsgDesc(cmd)

#define MSG_SERIALIZE(cmd,msg,pData)\
    PROTOCPP::CmdManager::msgSerialize(cmd,msg,pData)

#define MSG_DESERIALIZE(cmd,msg,pData,szie)\
    PROTOCPP::CmdManager::msgDeserialize(cmd,msg,pData,size)

#endif
