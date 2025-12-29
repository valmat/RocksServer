/**
 *  PluginABI.h
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once
#include <cstdint>

namespace RocksServer {

    struct PluginAbiVersion final {
        std::uint8_t  major = 1;
        std::uint8_t  minor = 0;
        std::uint16_t patch = 0;

        consteval std::uint32_t value() const noexcept
        {
            return (std::uint32_t{major} << 24)
                 | (std::uint32_t{minor} << 16)
                 |  std::uint32_t{patch};
        }
    };

    inline constexpr std::uint32_t pluginAbiVersion = PluginAbiVersion{}.value();

} // namespace RocksServer

extern "C" [[gnu::visibility("default")]]
std::uint32_t rocksserver_plugin_abi_version() noexcept;
