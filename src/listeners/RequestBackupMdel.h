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

        RequestBackupMdel(RocksDBWrapper &db, const rocksdb::BackupableDBOptions &bkOptions) :
            db(db),
            bkOptions(bkOptions)
        {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) override;

        virtual ~RequestBackupMdel() {}
    private:
        RocksDBWrapper& db;
        const rocksdb::BackupableDBOptions &bkOptions;
    };

}
