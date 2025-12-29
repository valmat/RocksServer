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

extern "C" std::uint32_t rocksserver_plugin_abi_version() noexcept
{
    return RocksServer::pluginAbiVersion;
}

namespace RocksServer {

    namespace {
        using abi_fn_t = std::uint32_t (*)() noexcept;
    }

    PlugContainer::PlugContainer(std::string plug_file, EvServer &server, RocksDBWrapper &rdb, const IniConfigs &cfg)
        : handle(dlopen(plug_file.c_str(), RTLD_NOW | RTLD_LOCAL))
    {
        if (!handle) {
            std::cerr << "Load extension " << plug_file << " failed\n"
                      << dlerror() << '\n';
            return;
        }

        // ABI check
        dlerror(); // clear
        auto abi_fn = reinterpret_cast<abi_fn_t>(dlsym(handle, "rocksserver_plugin_abi_version"));
        const char* err = dlerror();

        if (err == nullptr && abi_fn != nullptr) {
            const std::uint32_t plugin_abi = abi_fn();
            if (plugin_abi != pluginAbiVersion) {
                std::cerr << "Extension " << plug_file << " ABI mismatch. "
                          << "plugin=" << plugin_abi
                          << " server=" << pluginAbiVersion << '\n';
                dlclose(handle);
                handle = nullptr;
                return;
            }
        } else {
            std::cerr << "Extension " << plug_file
                      << " has no ABI version symbol; refused to load\n";
            dlclose(handle);
            handle = nullptr;
            return;
        }

        // Load required init function
        dlerror(); // clear
        plug_t plugin = nullptr;
        *reinterpret_cast<void **>(&plugin) = dlsym(handle, plug_fname);
        err = dlerror();

        if (err != nullptr || plugin == nullptr) {
            std::cerr << "Load extension " << plug_file << " failed\n"
                      << (err ? err : "dlsym returned null") << '\n';
            dlclose(handle);
            handle = nullptr;
            return;
        }

        (*plugin)(server, rdb, cfg);
    }

    PlugContainer::~PlugContainer()
    {
        if (handle) {
            dlclose(handle);
        }
    }

}
