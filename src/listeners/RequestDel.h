/**
 *  RequestDel.h
 *
 *  Request listener for command "/del"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestDel : public RequestBase
    {
    public:
        RequestDel(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       protocol object
         */
        virtual void run(const EvRequest &request, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !request.isPost() ) {
                EvLogger::writeLog("Request method should be POST");
                out.fail();
                return;
            }
            
            auto raw = request.getPostData();
            if(!raw.size()) {
                out.fail();
                return;
            }
                
            if( _rdb.del(rocksdb::Slice(raw, raw.size())) ) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestDel() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
