/**
 *  RequestBackupMdel.h
 *
 *  Request listener for command "/backup/del"
 *  Delete selected backups
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

using rocksdb::BackupableDB;

namespace RocksServer {

    class RequestBackupMdel : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:

        RequestBackupMdel(rocksdb::DB *rdb) : db(reinterpret_cast<BackupableDB*>(rdb)) {} 

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !in.check(out) ) {
                return;
            }

            for (auto &it : in) {
                // Get buckupID from the POST data
                uint32_t backup_id = std::strtoul(it.data(), nullptr, 10);
                // execute delete operation
                auto status = db->DeleteBackup(backup_id);

                if( status.ok() ) {
                    out.ok();
                } else {
                    out.fail();
                    // Log error message
                    EvLogger::writeLog(status.ToString().data());
                }
            }
        }

        virtual ~RequestBackupMdel() {}
    private:
        BackupableDB* db;
    };

}
