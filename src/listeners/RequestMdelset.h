/**
 *  RequestMdelset.h
 *
 *  Request listener for command "mdelset" (multi del and set)
 *  Allows to remove and set a keys for a single query
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestMdelset : public RequestBase<ProtocolInPostPairs, ProtocolOut>
    {
    public:
        RequestMdelset(RocksDBWrapper &rdb) : db(rdb) {}

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
            Batch batch;

            auto it = in.begin();
            auto itend = in.end();
            // Remove keys
            for(; it != itend && it->first.empty(); ++it) {
                batch.del(it->second);
            }
            // Set key-value pairs
            for(; it != itend; ++it) {
                batch.set(*it);
            }

            // Set batch to the DB and filling output buffer
            if(db.commit(batch)) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(db.getStatus().data());
            }
        }

        virtual ~RequestMdelset() {}
    private:
        RocksDBWrapper& db;
    };

}