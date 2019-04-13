/**
 *  RequestBackupDel.cpp
 *
 *  Request listener for command "/backup/del"
 *  Delete selected backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    /**
     *  Runs request listener
     *  @param       protocol in object
     *  @param       protocol out object
     */
    void RequestBackupDel::run(const ProtocolInPost &in, const ProtocolOut &out) noexcept
    {
        // Detect if current method is POST
        if( !in.check(out) ) {
            return;
        }

        BackupEngine bkEngine {bkOptions};
        // Check creating engine status
        if( !bkEngine ) {
            out.fail();
            EvLogger::writeLog(bkEngine.status().ToString().data());
            return;
        }

        // Get buckupID from the POST data
        uint32_t backup_id = std::strtoul(in.key().data(), nullptr, 10);
        // execute delete operation
        auto status = bkEngine.delBackup(backup_id);

        if( status.ok() ) {
            out.ok();
        } else {
            out.fail();
            // Log error message
            EvLogger::writeLog(status.ToString().data());
        }
    }

}
