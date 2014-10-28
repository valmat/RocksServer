/**
 *  RequestIncr.h
 *
 *  Request listener for command "/incr"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestIncr : public RequestBase<ProtocolIn, ProtocolOut>
    {
    public:
        RequestIncr(RocksDBWrapper &rdb) : _rdb(rdb) {}
        
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

            bool rez;
            auto seppos = raw.find('&');
            auto len = raw.size();

            if( seppos < len-1 ) {
                rez = _rdb.incr( rocksdb::Slice( raw, seppos ),  rocksdb::Slice( raw+seppos+1, len - seppos - 1 ) );
            } else {
                rez = _rdb.incr( rocksdb::Slice(raw, raw.size()), "1" );
            }
            
            if( rez ) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestIncr() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
