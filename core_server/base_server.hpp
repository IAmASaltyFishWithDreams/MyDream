#ifndef __SERVER_CORE_BASE_SERVER__
#define __SERVER_CORE_BASE_SERBER__

#include <iostream>
#include "../common/macro.hpp"
NS_SERVICE_CORE_BEGIN

class Base_server
{
public:
    Base_server();
    ~Base_server();
public :
    static Base_server * getInstance();
    void setInterrupt() { m_isInterruptFlag = true; }
    bool getInterrupt() { return m_isInterruptFlag; }

public:
    /* business layer */
    /* get Name server*/
    virtual char * getServerName() = 0;
    /* init sever*/
    virtual bool onInitStart() = 0;
    /* close server*/
    virtual bool onCloseServer() = 0;
    /* run server*/
    virtual void onRunServer() = 0;

public:
    /*logic layer*/
    /* server operator*/
    virtual bool initStart() = 0;
    /*close server*/
    virtual bool closeServer() = 0;
    /* run main(main body)*/
    virtual bool runServer() = 0;
    /* init signal */
    virtual bool initSignal() = 0;
    /* interrupt server*/
    virtual void interruptServer() = 0;

private:
    static Base_server * m_server;          // server
    bool m_isInterruptFlag = false;         // sign 1 byte in win32
};
/* memory size 1 + 1 + 2 = 4 byte. ps: 4 constitute 1(translater)+ 1(m_isInterruptFlag) +2(alignment)*/
NS_SERVICE_CORE_END
#endif
