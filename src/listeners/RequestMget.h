/**
 *  RequestMget.h
 *
 *  Request listener for command "mget" (multiget)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestMget : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestMget(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) override
        {
            // If no key is not transferred
            if(!in.check()) {
                return;
            }
            
            std::vector<rocksdb::Slice> keys;
            for (auto &it : in) {
                keys.emplace_back( it );
            }

            // Retrive result
            std::vector<rocksdb::Status> statuses;
            std::vector<std::string> values = _rdb.mget(keys, statuses);

            // filling buffer
            for(unsigned i=0; i < keys.size(); i++) {
                if(statuses[i].ok()) {
                    out.setPair(keys[i], values[i]);
                } else {
                    out.setFailPair(keys[i]);
                }
            }
        }

        virtual ~RequestMget() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
