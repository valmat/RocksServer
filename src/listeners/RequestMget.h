/**
 *  RequestMget.h
 *
 *  Request listener for command "mget" (multiget)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestMget : public RequestBase
    {
    public:
        RequestMget(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolIn &in, const ProtocolOut &out) override
        {
            std::string uri = in.getUri();
            const size_t pathlen = uri.find('?');   // length of "/mget?"
            const size_t len = uri.size();

            // If no key is not transferred
            if(len-1 <= pathlen) {
                return;
            }
            
            std::string::size_type lpos = pathlen+1;
            std::string::size_type rpos = uri.find('&', lpos);

            const char * uri_str = uri.c_str();

            // filling keys
            std::vector<rocksdb::Slice> keys;
            while(rpos < std::string::npos) {
                keys.emplace_back( rocksdb::Slice(uri_str+lpos, rpos-lpos) );
                lpos = rpos+1;
                rpos = uri.find('&', lpos);
            }
            keys.emplace_back( rocksdb::Slice(uri_str+lpos, len-lpos) );

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
