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
        RequestMset(RocksDBWrapper &rdb) : db(rdb) {}

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

            // Set batch to the DB and filling output buffer
            if(db.mset(batch)) {
                out.ok();
            } else {
                out.fail(); 
                EvLogger::writeLog(db.getStatus().data());
            }
        }

        virtual ~RequestMset() {}
    private:
        RocksDBWrapper& db;
    };

}
