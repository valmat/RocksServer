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

int main(int argc, char **argv)
{
    
    if (argc < 2) {
        std::cout << "Run as:" << std::endl << argv[0] << " <config.ini file name>" << std::endl;
        return 1;
    }

    /**
     *  
     *  Init config
     *  
     */
    RocksServer::IniConfigs cfg(argv[1]);
    if(!cfg) {
        std::cerr<<"Error with open file "<< argv[1] << std::endl;
        return 1;
    }

    /**
     *  
     *  Init libevent
     *  
     */
    if (!event_init()) {
        std::cerr << "Failed to init libevent." << std::endl;
        return 1;
    }
  
    /**
     *  
     *  Init event http server
     *  
     */
    EvServer server(cfg.get<const char *>("server_host", "127.0.0.1"), 
                    cfg.get<unsigned short>("server_port", 5577));

  
    /**
     *  
     *  Check server started
     *  
     */
    if (!server.isValid()) {
        std::cerr << "Failed to init http server." << std::endl;
        return 1;
    }

    /**
     *  
     *  Set http server options
     *  
     */
    EvServerOptions serverOptions;
    //Set the value to use for the Content-Type header when none was provided. 
    serverOptions.content_type = cfg.get<const char *>("content_type", "text/plain; charset=UTF-8");
    // Sets the what HTTP methods are supported in requests accepted by this server
    serverOptions.allowed_methods = EVHTTP_REQ_POST | EVHTTP_REQ_GET;
    // Limitations for body size (limit in bytes. 0 - unlimited)    
    serverOptions.max_body_size = cfg.get<size_t>("max_body_size", 0);
    // Limitations for headers size (limit in bytes. 0 - unlimited)    
    serverOptions.max_headers_size = cfg.get<size_t>("max_headers_size", 0);
    server.setOptions(serverOptions);

    /**
     *  
     *  Init RocksDB
     *  
     */
    RocksServer::RocksDBWrapper rdb(cfg.get<std::string>("db_path", "/tmp/rdb"));

    /**
     *  
     *  Bind request listeners
     *  
     */
    server.onRequest("/get",   RequestGet(rdb));
    server.onRequest("/mget",  RequestMget(rdb));
    server.onRequest("/set",   RequestSet(rdb));
    server.onRequest("/mset",  RequestMset(rdb));
    server.onRequest("/exist", RequestKeyExist(rdb));
    server.onRequest("/del",   RequestDel(rdb));
    server.onRequest("/mdel",  RequestMdel(rdb));
    server.onRequest("/incr",  RequestIncr(rdb));

    
    /**
     *  
     *  Start the event loop
     *  
     */
    if (event_dispatch() == -1) {
        std::cerr << "Failed to run messahe loop." << std::endl;
        return 1;
    }

    return 0;
}