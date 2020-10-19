#include "base_server.hpp"

#include <stddef.h>

NS_SERVICE_CORE_BEGIN

BaseServer * BaseServer::m_server = NULL;

BaseServer::BaseServer() {
    m_server = this;
}

BaseServer::~BaseServer() {
    m_server = NULL;
}

BaseServer * BaseServer :: getInstance() {
    return m_server;
}

NS_SERVICE_CORE_END
