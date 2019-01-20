/**
 *  RequestBackupDel.h
 *
 *  Request listener for command "/backup/del"
 *  Delete selected backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RequestBackupDel : public RequestBase<ProtocolInPost, ProtocolOut>
    {
    public:

        RequestBackupDel(RocksDBWrapper &db, const rocksdb::BackupableDBOptions &bkOptions) :
            db(db),
            bkOptions(bkOptions)
        {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPost &in, const ProtocolOut &out) override;

        virtual ~RequestBackupDel() {}
    private:
        RocksDBWrapper& db;
        const rocksdb::BackupableDBOptions &bkOptions;
    };

}
