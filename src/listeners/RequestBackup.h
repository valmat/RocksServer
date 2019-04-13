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

namespace RocksServer {

    class RequestBackup : public RequestBase<ProtocolInPostSimple, ProtocolOut>
    {
    public:

        RequestBackup(
            RocksDBWrapper &rdb,
            const rocksdb::BackupableDBOptions &bkOptions,
            uint32_t num_backups,
            bool flush_before_backup
        ) noexcept ;

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostSimple &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestBackup() = default;
        
    private:
        RocksDBWrapper& db;

        const rocksdb::BackupableDBOptions &bkOptions;
        // num backups to keep
        uint32_t num_backups;

        bool flush_before_backup;
    };

}
