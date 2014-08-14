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
         *  @param       event request object
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &request, const EvBuffer &buf) override
        {
            std::string uri = request.getUri();
            //const std::string::size_type PATHLEN = sizeof("/mget?")-1;   // length of "/mget?"
            const size_t pathlen = uri.find('?');   // length of "/mget?"
            const size_t len = uri.size();

            // If no key is not transferred
            if(len-1 <= pathlen) {
                // Add buffer
                buf.add("", 0);
                return;
            }
            
            std::string::size_type lpos = pathlen+1;
            std::string::size_type rpos = uri.find('&', lpos);

            const char * uri_str = uri.c_str();

            // filling keys
            std::vector<rocksdb::Slice> keys;
            while(rpos < std::string::npos) {
                //std::cout << "[" << lpos << "\t" << rpos << "]" << std::endl;
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
                // Add buffer
                buf.add(keys[i].data(), keys[i].size());
                if(statuses[i].ok()) {
                    buf.add_printf("\n%lu\n", values[i].size());
                    buf.add(values[i].c_str(), values[i].size());
                    buf.add("\n", 1);
                } else {
                    // push NULL
                    buf.add("\n-1\n", 4);
                }
            }
            //std::cout << "RequestMget" << std::endl;
        }

        virtual ~RequestMget() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
