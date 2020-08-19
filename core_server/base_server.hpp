#ifndef __SERVER_CORE_BASE_SERVER__
#define __SERVER_CORE_BASE_SERBER__

#include "../common/macro.hpp"
USING_BASE_SERVER_BEGIN

class base_server
{
public:
    base_server() {}
    ~base_server() {}
public:
    /* business layer */
    virtual void  initStart() = 0;

private:

};
USING_BASE_SERVER_END
#endif
