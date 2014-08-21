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
         *  @param       event request object
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &request, const EvBuffer &buf) override
        {
            // Detect if current method is POST
            if( !request.isPost() ) {
                buf.add("Request method should be POST");
                return;
            }

            BackupEngine bk(_bk_path);
            auto status = bk.createBackup(_rdb);

            if( status.ok() ) {
                buf.add("OK", 2);
            } else {
                buf.add("FAIL\n", 5);
                buf.add(status.ToString());
            }

        }

        virtual ~RequestBackup() {}
    private:
        RocksDBWrapper& _rdb;

        std::string _bk_path;

    };

}
