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
    ExtendObj::ExtendObj(std::string plug_file, const EvServer &server, const RocksDBWrapper &rdb, const IniConfigs &cfg) :
        handle(dlopen(plug_file.data(), RTLD_LAZY))
    {
        //Check for errors
        if (!handle) {
            std::cerr << "Load extention " << plug_file << " missed" << std::endl;
            std::cerr <<  dlerror() << std::endl;;
            return;
        }

        // Load a required function.
        plug_t plugin = (plug_t) dlsym(handle, "myplug");
        //Check for errors
        if (!plugin) {
            std::cerr << "Load extention " << plug_file << " missed" << std::endl;
            std::cerr <<  dlerror() << std::endl;
            dlclose(handle);
            handle = nullptr;
            return;
        }

        // Execute the function from the extension
        std::cout << "Исполняем функцию из библиотеки: " << plugin(rdb, cfg) << std::endl;

    }

    ExtendObj::~ExtendObj()
    {
        // Close the extension
        std::cerr <<  "Закрываем библиотеку" << std::endl;
        if(handle) dlclose(handle);
    }


}