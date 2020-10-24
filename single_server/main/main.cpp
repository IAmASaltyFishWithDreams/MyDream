#include "server_global.hpp"
#include "single_server.hpp"
#include "../../utils/log.hpp"

ServerGlobla * global::pServer = NULL;

int main()
{
    SingleServer  server;
    global::pServer = & server;

    if (!global::pServer->runServer()) {
        log_error("****************");
        return;
    }
}

