/**
 *  RequestMdel.h
 *
 *  Request listener for command "mset" (multiset)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestMdel : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:
        RequestMdel(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if(!in.check(out)) {
                return;
            }
            
            // create a RocksDB delete batch
            rocksdb::WriteBatch batch;
            for (auto &it : in) {
                batch.Delete(it);
            }

            // Apply the delete-batch to the RocksDB
            if(_rdb.mset(batch)) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestMdel() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
