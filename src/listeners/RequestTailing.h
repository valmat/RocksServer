/**
 *  RequestTailing.h
 *
 *  Request listener for command "tail" (Tailing iterator)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestTailing : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
    public:
        RequestTailing(RocksDBWrapper &rdb) : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override
        {
            auto rOpt = rocksdb::ReadOptions();
            rOpt.tailing = true;
            std::unique_ptr<rocksdb::Iterator> it(db->NewIterator(std::move(rOpt)));

            // filling buffer
            for (it->SeekToFirst(); it->Valid(); it->Next()) {
                if(it->status().ok()) {
                    out.setPair(it->key(), it->value());
                } else {
                    out.setFailPair(it->key());
                }
            }
        }

        virtual ~RequestTailing() {}
    private:
        RocksDBWrapper& db;
    };

}
