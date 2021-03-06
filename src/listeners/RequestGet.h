/**
 *  RequestGet.h
 *
 *  Request listener for command "get"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestGet : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestGet(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestGet() = default;
    private:
        RocksDBWrapper& db;
    };

}
