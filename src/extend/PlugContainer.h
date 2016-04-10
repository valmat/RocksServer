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
        using plug_t = void (*)(Extension, RocksDBWrapper&, const IniConfigs&);

        // A function pointer obtained through dlopen
        void* handle;
        // Plugin function name
        const char* plug_fname = "plugin";
    };
}