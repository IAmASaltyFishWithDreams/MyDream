#include "cmd_manager.hpp"

#include "template_cmd.hpp"

NS_PROTOCPP_BEGIN
#define DESC_MSG(msg)   m_descMap.inserter(std::make_pair(msg,#msg))

std::map<uint32, std::string> CmdManager::m_descMap;

void CmdManager::init(){
    m_descMap.clear();

    %for _,value in Maps:
        DESC_MSG(${value});
    %endfor
}

void CmdManager::clear() {
    m_descMap.clear();
}

const char* getMsgDesc(uint32 cmd) {
    auto iter = m_descMap.find(cmd);
    if (iter != m_descMap.end()) {
        return iter->second.c_str();
    }
    return NULL;
}

NS_PROTOCPP_END
