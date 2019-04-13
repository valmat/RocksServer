/**
 *  RequestMdelset.h
 *
 *  Request listener for command "mdelset" (multi del and set)
 *  Allows to remove and set a keys for a single query
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestMdelset : public RequestBase<ProtocolInPostPairs, ProtocolOut>
    {
    public:
        RequestMdelset(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostPairs &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestMdelset() = default;
    private:
        RocksDBWrapper& db;
    };

}