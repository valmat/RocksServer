/**
 *  RequestStats.h
 *
 *  Request listener for command "stats" (Get statistics)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestStats : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
    public:
        RequestStats(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestStats() = default;
    private:
        RocksDBWrapper& db;
    };

}
