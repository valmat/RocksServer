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

namespace RocksServer {

    class RequestBackupInfo : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
    public:
        
        RequestBackupInfo(const rocksdb::BackupableDBOptions &bkOptions) :
            bkOptions(bkOptions)
        {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override;

        virtual ~RequestBackupInfo() {}
    private:
        const rocksdb::BackupableDBOptions &bkOptions;
    };
}
