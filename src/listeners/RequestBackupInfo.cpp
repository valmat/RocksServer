/**
 *  RequestBackupInfo.cpp
 *
 *  Request listener for command "/backup/info"
 *  Show information about backups
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"
#include <time.h>

namespace RocksServer {

    /**
     *  Runs request listener
     *  @param       protocol in object
     *  @param       protocol out object
     */
    void RequestBackupInfo::run(const ProtocolInTrivial &in, const ProtocolOut &out) noexcept
    {
        BackupEngine bkEngine {bkOptions};
        // Check creating engine status
        if( !bkEngine ) {
            out.fail();
            EvLogger::writeLog(bkEngine.status().ToString().data());
            return;
        }

        std::vector<rocksdb::BackupInfo> backup_info = bkEngine.backupInfo();
        
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

}
