/**
 *  RequestIncr.h
 *
 *  Request listener for command "/incr"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestIncr : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:
        RequestIncr(RocksDBWrapper &rdb) : _rdb(rdb) {}
        
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !in.check(out) ) {
                return;
            }

            bool rez;
            auto it = in.begin();
            auto key = *it;
            ++it;

            rez = ( in.end() !=  it) ? _rdb.incr( key,  *it ) : _rdb.incr( key );
            
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
