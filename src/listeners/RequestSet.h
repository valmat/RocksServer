/**
 *  RequestSet.h
 *
 *  Request listener for command "set"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestSet : public RequestBase
    {
    public:
        RequestSet(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &request, const EvBuffer &buf) override
        {
            auto raw = request.getPostData();

            // retrive key and value
            std::string::size_type lpos = 0;
            std::string::size_type rpos = raw.find('\n');
            rocksdb::Slice key(raw, rpos);
            
            lpos = rpos+1;
            rpos = raw.find('\n', lpos);
            long vallen = std::atol(raw + lpos);
            lpos = rpos+1;
            rocksdb::Slice value(raw + lpos, vallen);

            // set and filling buffer
            if(_rdb.set(key, value)) {
                buf.add("OK", 2); 
            } else {
                buf.add("FAIL", 4); 
            }
            //std::cout << "RequestSet" << std::endl;
        }

        virtual ~RequestSet() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
