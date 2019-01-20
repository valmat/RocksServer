/**
 *  RequestWstats.h
 *
 *  Request listener for command "wstats" (Get wide statistics)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestWstats : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestWstats(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) override;

        virtual ~RequestWstats() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
