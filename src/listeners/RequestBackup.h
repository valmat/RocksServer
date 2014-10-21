/**
 *  RequestBackup.h
 *
 *  Request listener for command "/backup"
 *  Create backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestBackup : public RequestBase
    {
    public:
        
        RequestBackup(RocksDBWrapper &rdb, const std::string &bk_path) : _rdb(rdb), _bk_path(bk_path) {} 

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolIn &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !request.isPost() ) {
                EvLogger::writeLog("Request method should be POST");
                out.fail();
                return;
            }

            BackupEngine bk(_bk_path);
            auto status = bk.createBackup(_rdb);

            if( status.ok() ) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(status.ToString().data());
            }
        }

        virtual ~RequestBackup() {}
    private:
        RocksDBWrapper& _rdb;

        std::string _bk_path;

    };

}
