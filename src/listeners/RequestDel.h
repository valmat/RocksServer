/**
 *  RequestDel.h
 *
 *  Request listener for command "/del"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestDel : public RequestBase<ProtocolIn, ProtocolOut>
    {
    public:
        RequestDel(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolIn &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !in.checkPost(out) || !in.checkPostSize(out) ) {
                return;
            }
            
            auto raw = in.getRawPost();
                
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
