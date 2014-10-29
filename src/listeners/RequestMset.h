/**
 *  RequestMset.h
 *
 *  Request listener for command "mset" (multiset)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestMset : public RequestBase<ProtocolInPostPairs, ProtocolOut>
    {
    public:
        RequestMset(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostPairs &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !in.check(out)) {
                return;
            }
            
            // create a RocksDB write batch
            rocksdb::WriteBatch batch;
            for (auto &it : in) {
                batch.Put(it.first, it.second);
            }

            // set and filling buffer
            if(_rdb.mset(batch)) {
                out.ok();
            } else {
                out.fail(); 
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestMset() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
