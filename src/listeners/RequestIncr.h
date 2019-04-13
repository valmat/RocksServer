/**
 *  RequestIncr.h
 *
 *  Request listener for command "/incr"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestIncr : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:
        RequestIncr(RocksDBWrapper &rdb) noexcept : db(rdb) {}
        
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestIncr() = default;
    private:
        RocksDBWrapper& db;
    };

}
