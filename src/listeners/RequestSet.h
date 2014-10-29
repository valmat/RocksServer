/**
 *  RequestSet.h
 *
 *  Request listener for command "set"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestSet : public RequestBase<ProtocolInPost, ProtocolOut>
    {
    public:
        RequestSet(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPost &in, const ProtocolOut &out) override
        {
            // Detect if current method is correct POST
            if( !in.check(out) ) {
                return;
            }
            
            auto pair = in.pair();

            if(_rdb.set(pair.first, pair.second)) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestSet() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
