/**
 *  RequestGetIncr.h
 *
 *  Request listener for command "/get-incr"
 *  Increments value and returns the result
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once
 
namespace RocksServer {

    class RequestGetIncr : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:
        RequestGetIncr(RocksDBWrapper &db) noexcept : db(db) {}
        
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) noexcept override;

        virtual ~RequestGetIncr() {}
    private:
        RocksDBWrapper& db;
    };

}
