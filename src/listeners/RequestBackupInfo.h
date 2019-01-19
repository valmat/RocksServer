/**
 *  RequestBackupInfo.h
 *
 *  Request listener for command "/backup/info"
 *  Show information about backups
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once
#include <time.h>

namespace RocksServer {

    class RequestBackupInfo : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
    public:
        
        RequestBackupInfo(RocksDBWrapper &rdb, const rocksdb::BackupableDBOptions &bkOptions) :
            db(rdb),
            bkOptions(bkOptions)
        {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override
        {
            std::vector<rocksdb::BackupInfo> backup_info = BackupEngine(bkOptions).backupInfo();
            
            out.setStr(backup_info.size());
            struct tm * dt;
            char buffer [26];            
            for(auto inf: backup_info) {
                dt = localtime(&inf.timestamp);
                strftime(buffer, sizeof(buffer), "%d.%m.%Y %T %z", dt);

                out.setStr(std::string("\nid: ")      + std::to_string(inf.backup_id) + "\n");
                out.setStr(std::string("timestamp: ") + std::to_string(inf.timestamp) + "\n");
                out.setStr(std::string("time: ")      + std::string(buffer)           + "\n");
                out.setStr(std::string("size: ")      + std::to_string(inf.size)      + "\n");
            }
        }

        virtual ~RequestBackupInfo() {}
    private:
        RocksDBWrapper& db;
        const rocksdb::BackupableDBOptions &bkOptions;
    };
}
