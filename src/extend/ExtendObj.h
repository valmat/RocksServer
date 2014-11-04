/**
 *
 *  ExtendObj.h
 *  ExtendObj RocksServer
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

#include <dlfcn.h>

namespace RocksServer {

    struct ExtendObj
    {
        // Define plugin type  
        typedef int (*plug_t)(const RocksDBWrapper&, const IniConfigs&);

        ExtendObj(std::string plug_file, const EvServer &server, const RocksDBWrapper &rdb, const IniConfigs &cfg);

        ~ExtendObj();

    private:
        void* handle;
    };

}