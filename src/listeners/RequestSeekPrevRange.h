/**
 *  RequestSeekPrevRange.h
 *
 *  Request listener for command "seekprev-range"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestSeekPrevRange : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestSeekPrevRange(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestSeekPrevRange() = default;
    private:
        RocksDBWrapper& db;
    };

}
