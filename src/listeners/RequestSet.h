/**
 *  RequestSet.h
 *
 *  Request listener for command "set"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestSet : public RequestBase<ProtocolInPost, ProtocolOut>
    {
    public:
        RequestSet(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPost &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestSet() = default;
    private:
        RocksDBWrapper& db;
    };

}
