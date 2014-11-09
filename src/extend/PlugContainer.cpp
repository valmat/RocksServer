/**
 *
 *  PlugContainer.cpp
 *  Plugin container
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"
#include <dlfcn.h>

namespace RocksServer {
    PlugContainer::PlugContainer(std::string plug_file, EvServer &server, RocksDBWrapper &rdb, const IniConfigs &cfg) :
        handle(dlopen(plug_file.data(), RTLD_LAZY))
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

        // Execute the function from an extension
        plugin(server, rdb, cfg);
    }

    PlugContainer::~PlugContainer()
    {
        // Close the extension
        if(handle) dlclose(handle);
    }


}