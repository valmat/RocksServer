/**
 *  RequestBackupDel.h
 *
 *  Request listener for command "/backup/del"
 *  Delete selected backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

using rocksdb::BackupableDB;

namespace RocksServer {

    class RequestBackupDel : public RequestBase<ProtocolInPost, ProtocolOut>
    {
    public:


        RequestBackupDel(rocksdb::DB *rdb) : db(reinterpret_cast<BackupableDB*>(rdb)) {} 

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPost &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !in.check(out) ) {
                return;
            }

            uint32_t backup_id = std::strtoul(in.key().data(), nullptr, 10);
            auto status = db->DeleteBackup(backup_id);

            if( status.ok() ) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(status.ToString().data());
            }
        }

        virtual ~RequestBackupDel() {}
    private:
        BackupableDB* db;
    };

}
