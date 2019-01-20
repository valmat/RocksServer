/**
 *  RequestSeekFirst.h
 *
 *  Request listener for command "/seek-first"
 *  Returns the first pair corresponding to the key prefix
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/queueServer
 */
#pragma once

namespace RocksServer {

    class RequestSeekFirst : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestSeekFirst(RocksDBWrapper &db) noexcept : db(db) {}
        
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) noexcept override;

        virtual ~RequestSeekFirst() {}
    private:
        RocksDBWrapper& db;
    };

}
