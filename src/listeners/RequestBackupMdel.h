/**
 *  RequestBackupMdel.h
 *
 *  Request listener for command "/backup/mdel"
 *  Delete selected backups
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestBackupMdel : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:

        RequestBackupMdel(const rocksdb::BackupableDBOptions &bkOptions) noexcept :
            bkOptions(bkOptions)
        {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) noexcept final;

        virtual ~RequestBackupMdel() = default;
    private:
        const rocksdb::BackupableDBOptions &bkOptions;
    };

}
