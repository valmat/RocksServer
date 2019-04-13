/**
 *  RequestMget.h
 *
 *  Request listener for command "mget" (multiget)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestMget : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestMget(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestMget() = default;
    private:
        RocksDBWrapper& db;
    };

}
