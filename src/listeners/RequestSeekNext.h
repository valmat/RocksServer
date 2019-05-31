/**
 *  RequestSeekNext.h
 *
 *  Request listener for command "seeknext"
 *  https://github.com/facebook/rocksdb/wiki/SeekForPrev
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestSeekNext : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestSeekNext(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestSeekNext() = default;
    private:
        RocksDBWrapper& db;
    };

}
