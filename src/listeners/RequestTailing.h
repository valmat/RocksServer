/**
 *  RequestTailing.h
 *
 *  Request listener for command "tail" (Tailing iterator)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestTailing : public RequestBase
    {
    public:
        RequestTailing(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       protocol object
         */
        virtual void run(const EvRequest &request, const ProtocolOut &out) override
        {
            rocksdb::Slice key, value;
            auto rOpt = rocksdb::ReadOptions();
            rOpt.tailing = true;
            rocksdb::Iterator* it = _rdb->NewIterator(std::move(rOpt));
            for (it->SeekToFirst(); it->Valid(); it->Next()) {
                key   = it->key();
                value = it->value();
                out.setPair(key, value);
            }
        }

        virtual ~RequestTailing() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
