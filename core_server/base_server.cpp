#include "base_server.hpp"
NS_SERVICE_CORE_BEGIN

static Base_server * m_server = NULL;

Base_server::Base_server() {
    m_server = this;
}

Base_server::~Base_server() {
    m_server = NULL;
}

Base_server * Base_server :: getInstance() {
    return m_server;
}

NS_SERVICE_CORE_END
