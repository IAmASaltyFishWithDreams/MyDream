#ifndef __SINGLE_SERVER_SERVER_GLOBAL_HPP__
#define __SINGLE_SERVER_SERVER_GLOBAL_HPP__

#define LINK_ACTIVE_TIME 10
#define LINK_OUTTIME 15

class SingleServer;

struct global{
    static SingleServer* pServer;
};
#endif
