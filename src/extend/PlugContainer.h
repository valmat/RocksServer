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

    class PlugContainer
    {
    public:
        PlugContainer(std::string plug_file, EvServer &server, RocksDBWrapper &rdb, const IniConfigs &cfg);

        ~PlugContainer();

    private:
        // Define plugin type  
        typedef void (*plug_t)(Extension, RocksDBWrapper&, const IniConfigs&);

        void* handle;
        const char* plug_fname = "plugin";
    };
}