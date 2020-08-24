#ifndef __SERVER_CORE_BOOST_SEVER__
#define __SERVER_CORE_BOOST_SEVER__

#include "base_server.hpp"
#include "../boost_network/network.hpp"
#include "../common/macro.hpp"

NS_SERVICE_CORE_BEGIN
/**
 *  three layer server
*/
class Boost_server : public Base_server
{
public:
    Boost_server() {}
    ~Boost_server() {}
public:
    /* specific server */
    /* network layer */

    bool startNetwork(int nthread);

    bool listenAt(const std::string& ip, int port, AcceptorCallback* pAcceptorCb);

    bool connectTo(const std::string& ip, int port, ConnectorCallback* pConnectorCb, int reConnectTime);

    void stopNetwork();

    void postCloseSocket(const TcpSocketPtr& ptr);

private:


private:
    /*TODO logic layer*/

    /*net data*/
    BOOST_NETWORK::BoostNetwork m_network;
};

#endif
NS_SERVICE_CORE_END
