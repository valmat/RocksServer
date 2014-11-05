/**
 *
 *  Extend.h
 *  Extend RocksServer
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class Extend
    {
    public:
        void load(EvServer &server, RocksDBWrapper &rdb, const IniConfigs &cfg, const DefaultConfigs &dfCfg);

    private:
        std::forward_list<PlugContainer> handles;
    };

}