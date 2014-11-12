/**
 *  RequestBackupInfo.h
 *
 *  Request listener for command "/backup"
 *  Create backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once
#include <time.h>

using rocksdb::BackupableDB;

namespace RocksServer {

    class RequestBackupInfo : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
    public:
        
        RequestBackupInfo(rocksdb::DB *rdb) : db(reinterpret_cast<BackupableDB*>(rdb)) {} 

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override
        {
            struct tm * dt;
            char buffer [26];

            std::vector<rocksdb::BackupInfo> backup_info;
            db->GetBackupInfo(&backup_info);
            for(auto inf: backup_info) {
                dt = localtime(&inf.timestamp);
                strftime(buffer, sizeof(buffer), "%d.%m.%Y %T %z", dt);

                std::cout << "id: " << inf.backup_id << std::endl;
                std::cout << "timestamp: " << inf.timestamp << std::endl;
                std::cout << "time: " << buffer << std::endl;
                std::cout << "size: " << inf.size << std::endl;
                std::cout << std::endl;

                out.setStr(std::string("id: ") + std::to_string(inf.backup_id) + "\n");
                out.setStr(std::string("timestamp: ") + std::to_string(inf.timestamp) + "\n");
                out.setStr(std::string("time: ") + std::string(buffer) + "\n");
                out.setStr(std::string("size: ") + std::to_string(inf.size) + "\n\n");
            }
        }

        virtual ~RequestBackupInfo() {}
    private:
        BackupableDB* db;
    };
}
