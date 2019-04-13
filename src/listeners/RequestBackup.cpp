/**
 *  RequestBackup.cpp
 *
 *  Request listener for command "/backup"
 *  Create backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    RequestBackup::RequestBackup(
        RocksDBWrapper &rdb,
        const rocksdb::BackupableDBOptions &bkOptions,
        uint32_t num_backups,
        bool flush_before_backup
    ) noexcept : 
        db(rdb),
        bkOptions(bkOptions),
        num_backups(num_backups),
        flush_before_backup(flush_before_backup)
    {} 

    /**
     *  Runs request listener
     *  @param       protocol in object
     *  @param       protocol out object
     */
    void RequestBackup::run(const ProtocolInPostSimple &in, const ProtocolOut &out) noexcept
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

        // Create backup
        auto status = bkEngine.createBackup(db, flush_before_backup);
        // Check creating backup status
        if( !status.ok() ) {
            out.fail();
            EvLogger::writeLog(status.ToString().data());
            return;
        }

        // if num_backups is specified, remove old backups
        if(num_backups) {
            status = bkEngine.purgeOldBackups(num_backups);
            if( !status.ok() ) {
                out.fail();
                EvLogger::writeLog(status.ToString().data());
                return;
            }
        }

        out.ok();
    }

}
