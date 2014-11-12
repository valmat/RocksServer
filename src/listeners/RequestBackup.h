/**
 *  RequestBackup.h
 *
 *  Request listener for command "/backup"
 *  Create backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

using rocksdb::BackupableDB;

namespace RocksServer {

    class RequestBackup : public RequestBase<ProtocolInPostSimple, ProtocolOut>
    {
    public:
        
        RequestBackup(rocksdb::DB *rdb) : db(reinterpret_cast<BackupableDB*>(rdb)) {} 

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostSimple &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !in.check(out) ) {
                return;
            }

            auto status = db->CreateNewBackup();

            if( status.ok() ) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(status.ToString().data());
            }
        }

        virtual ~RequestBackup() {}
    private:
        BackupableDB* db;
    };

}
