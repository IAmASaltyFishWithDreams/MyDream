#include <map>
#include <vector>


#include "acceptor.hpp"
#include "../utils/macro.hpp"
#include "../utils/thread_pool.hpp"

NS_BOOST_NETWORK_BEGIN
class BoostNetwork
{
public:
    BoostNetwork();
    ~BoostNetwork();
public:
    /*TODO tcp socket circuit*/
    void init(uint32 threadNum);
    
    /* close network*/
    void close();
    /*stop network*/
    void stop();
    /* lisen network*/
    bool listenAt(const std::string& ip, uint16 port, AcceptorCallback* pAcceptorCb);
    /* connect net work*/
    bool connectTO(const std::string& ip, uint16 port, ConnectorCallback* pConnectorCb, uint32 reConnectTime);
    /* close socket*/
    void postCloseSocket( const TcpSocketPtr& s);

protected:
    /*stop listen netWork*/
    void stopListen();
    /*clear listen*/
    void clearListen();
    /*interrupt connect*/
    void stopConnect();
    /*clear connect*/
    void clearConnect();

private:
    boost::asio::io_service m_ios;                                  /* server instance*/
    boost::shared_ptr<boost::asio::io_service::work> m_workPtr;     /* server control*/
    UTILS::ThreadPool m_threadPool;                                /* thread pool*/

    std::map<int, AcceptorPtr> m_lisetenMap;                        /* map acceptor */
    std::vector<ConnectorPtr> m_connectVec;                         /* vec connect*/
};
NS_BOOST_NETWORK_END
