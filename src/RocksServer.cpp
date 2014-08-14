/**
 *  RocksServer.cpp
 *
 *  Simple RocksDB server
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

using namespace RocksServer;

int main()
{
    if (!event_init()) {
        std::cerr << "Failed to init libevent." << std::endl;
        return -1;
    }
  
    // Init event http server
    EvServer server("127.0.0.1", 5577);
  
    // Check server started
    if (!server.isValid()) {
        std::cerr << "Failed to init http server." << std::endl;
        return -1;
    }

    // Set http server options
    EvServerOptions serverOptions;
    serverOptions.content_type = "text/plain; charset=UTF-8";
    serverOptions.allowed_methods = EVHTTP_REQ_POST | EVHTTP_REQ_GET;
    server.setOptions(serverOptions);

    // Init RocksDB
    RocksServer::RocksDBWrapper rdb("/tmp/rdb");

    // Bind request listeners
    server.onRequest({"/get","/get/"}, RequestGet(rdb));
    server.onRequest("/mget",  RequestMget(rdb));
    server.onRequest("/set",   RequestSet(rdb));
    server.onRequest("/mset",  RequestMset(rdb));
    server.onRequest("/exist", RequestKeyExist(rdb));
    server.onRequest("/del",   RequestDel(rdb));
    server.onRequest("/mdel",  RequestMdel(rdb));
    server.onRequest("/incr",  RequestIncr(rdb));

    
    

    // Start the event loop.
    if (event_dispatch() == -1) {
        std::cerr << "Failed to run messahe loop." << std::endl;
        return -1;
    }

    return 0;
}