/**
 *  RequestSeekNextRange.h
 *
 *  Request listener for command "seeknext-range"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestSeekNextRange : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestSeekNextRange(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestSeekNextRange() = default;
    private:
        RocksDBWrapper& db;
    };

}
