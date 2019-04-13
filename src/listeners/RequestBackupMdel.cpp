/**
 *  RequestBackupMdel.cpp
 *
 *  Request listener for command "/backup/mdel"
 *  Delete selected backups
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
    void RequestBackupMdel::run(const ProtocolInPostKeys &in, const ProtocolOut &out) noexcept
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

        for (auto &it : in) {
            // Get buckupID from the POST data
            uint32_t backup_id = std::strtoul(it.data(), nullptr, 10);
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

}
