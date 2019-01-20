/**
 *  RequestKeyExist.h
 *
 *  Request listener for command "exit" (check if key exist)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestKeyExist : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestKeyExist(RocksDBWrapper &rdb) : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) override;

        virtual ~RequestKeyExist() {}
    private:
        RocksDBWrapper& db;
    };

}
