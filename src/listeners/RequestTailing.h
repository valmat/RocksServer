/**
 *  RequestTailing.h
 *
 *  Request listener for command "tail" (Tailing iterator)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestTailing : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
    public:
        RequestTailing(RocksDBWrapper &rdb) noexcept : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestTailing() = default;
    private:
        RocksDBWrapper& db;
    };

}
