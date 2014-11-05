/**
 *
 *  PlugContainer.h
 *  Plugin container
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    struct PlugContainer
    {
        // Define plugin type  
        typedef void (*plug_t)(Extension&, const RocksDBWrapper&, const IniConfigs&);

        PlugContainer(std::string plug_file, EvServer &server, const RocksDBWrapper &rdb, const IniConfigs &cfg);

        ~PlugContainer();

    private:
        void* handle;
        const char * plug_fname = "plugin";
    };
}