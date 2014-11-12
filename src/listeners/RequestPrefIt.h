/**
 *  RequestPrefIt.h
 *
 *  Request listener for command "prefit"
 *  Prefix iterator
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestPrefIt : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestPrefIt(RocksDBWrapper &rdb) : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) override
        {
            // Check if any key is transferred
            if(!in.check()) {
                return;
            }
            
            auto prefix = in.key();
            std::unique_ptr<rocksdb::Iterator> iter(db->NewIterator(rocksdb::ReadOptions()));
            
            // Iterate over prefixed keys
            for (iter->Seek(prefix); iter->Valid() && iter->key().starts_with(prefix); iter->Next()) {
                if(iter->status().ok()) {
                    out.setPair(iter->key(), iter->value());
                } else {
                    out.setFailPair(iter->key());
                }
            }
        }

        virtual ~RequestPrefIt() {}
    private:
        RocksDBWrapper& db;
    };

}
