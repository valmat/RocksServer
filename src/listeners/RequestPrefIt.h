/**
 *  RequestPrefIt.h
 *
 *  Request listener for command "prefit"
 *  Prefix iterator
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestPrefIt : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestPrefIt(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestPrefIt() = default;
    private:
        RocksDBWrapper& db;
    };

}
