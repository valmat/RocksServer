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
    std::cout << "RocksServer version is " << ROCKSSERVER_VERSION << std::endl;

    /**
     *  
     *  Prevent server crash on signal SIGPIPE
     *  
     */    
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
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
     *  Init RocksDB
     *  
     */
    RocksServer::RocksDBWrapper rdb(cfg.get<std::string>("db_path", "/var/rocksserver/db"));
    // Check RocksDB started
    if (!rdb.status()) {
        std::cerr << "RocksDB start error:" << std::endl << rdb.getStatus() << std::endl;
        return 1;
    }
    std::cout << "RocksDB version is " << ROCKSDB_MAJOR << "." << ROCKSDB_MINOR << "." << ROCKSDB_PATCH << std::endl;

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

    // Check server started
    if (!server) {
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
     *  Bind request listeners
     *  
     */
    server.onRequest("/get",   new RequestGet(rdb));
    server.onRequest("/mget",  new RequestMget(rdb));
    server.onRequest("/set",   new RequestSet(rdb));
    server.onRequest("/mset",  new RequestMset(rdb));
    server.onRequest("/exist", new RequestKeyExist(rdb));
    server.onRequest("/del",   new RequestDel(rdb));
    server.onRequest("/mdel",  new RequestMdel(rdb));
    server.onRequest("/incr",  new RequestIncr(rdb));

    /**
     *  
     *  Start the event loop
     *  
     */
    if (!server.dispatch()) {
        std::cerr << "Failed to run messahe loop." << std::endl;
        return 1;
    }

    return 0;
}