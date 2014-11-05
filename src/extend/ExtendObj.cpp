/**
 *
 *  ExtendObj.cpp
 *  ExtendObj RocksServer
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"
#include <dlfcn.h>

namespace RocksServer {
    ExtendObj::ExtendObj(std::string plug_file, EvServer &server, const RocksDBWrapper &rdb, const IniConfigs &cfg) :
        handle(dlopen(plug_file.data(), RTLD_LAZY))
        //handle(dlopen(plug_file.data(), RTLD_NOW | RTLD_LOCAL))
    {
        //Check for errors
        if (!handle) {
            std::cerr << "Load extention " << plug_file << " missed" << std::endl;
            std::cerr <<  dlerror() << std::endl;;
            return;
        }

        // Load a required function.
        plug_t plugin = (plug_t) dlsym(handle, plug_fname);
        //Check for errors
        if (!plugin) {
            std::cerr << "Load extention " << plug_file << " missed" << std::endl;
            std::cerr <<  dlerror() << std::endl;
            dlclose(handle);
            handle = nullptr;
            return;
        }

        Extension extension;

        // Execute the function from the extension
        std::cout << "Исполняем функцию из библиотеки: " << std::endl;
        plugin(rdb, cfg, &extension);

        for(auto &it: extension) {
            server.onRequest(it.first.data(), it.second);
        }

    }

    ExtendObj::~ExtendObj()
    {
        // Close the extension
        std::cerr <<  "~ExtendObj" << std::endl;
        if(handle) dlclose(handle);
    }


}