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
        Extend(EvServer &server, const RocksDBWrapper &rdb, const IniConfigs &cfg, const DefaultConfigs &dfCfg);

        ~Extend()
        {
            std::cerr <<  "~Extend" << std::endl;
        }

    private:
        std::forward_list<ExtendObj> handles;
    };

}