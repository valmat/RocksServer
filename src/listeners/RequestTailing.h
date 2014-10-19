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
        virtual void run(const EvRequest &request, const Protocol &prot) override
        {
            rocksdb::Slice key, value;
            auto rOpt = rocksdb::ReadOptions();
            rOpt.tailing = true;
            rocksdb::Iterator* it = _rdb->NewIterator(std::move(rOpt));
            for (it->SeekToFirst(); it->Valid(); it->Next()) {
                key   = it->key();
                value = it->value();

                // Add to buffer
                buf.add(key.data(), key.size());
                buf.add_printf("\n%lu\n", value.size());
                buf.add(value.data(), value.size());
                buf.add("\n", 1);
            }
        }

        virtual ~RequestTailing() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
