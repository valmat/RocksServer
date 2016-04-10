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


        RequestBackup(rocksdb::DB *rdb, uint32_t num_backups, bool flush_before_backup) : 
            db(reinterpret_cast<BackupableDB*>(rdb)),
            num_backups(num_backups),
            flush_before_backup(flush_before_backup)
        {} 

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

            auto status = db->CreateNewBackup(flush_before_backup);

            if( !status.ok() ) {
                out.fail();
                EvLogger::writeLog(status.ToString().data());
                return;
            }

            // if num_backups is specified, remove old backups
            if(num_backups) {
                status = db->PurgeOldBackups(num_backups);

                if( !status.ok() ) {
                    out.fail();
                    EvLogger::writeLog(status.ToString().data());
                    return;
                }
            }

            out.ok();
        }

        virtual ~RequestBackup() {}
    private:
        BackupableDB* db;
        // num backups to keep
        uint32_t num_backups;

        bool flush_before_backup;
    };

}
