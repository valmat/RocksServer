/**
 *  RequestMset.h
 *
 *  Request listener for command "mset" (multiset)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestMset : public RequestBase
    {
    public:
        RequestMset(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &request, const EvBuffer &buf) override
        {
            // Detect if current method is POST
            if( !request.isPost() ) {
                buf.add("Request method should be POST");
                return;
            }
            
            auto raw = request.getPostData();

            // create a RocksDB write batch
            rocksdb::WriteBatch batch;


            std::string::size_type lpos = 0;
            std::string::size_type rpos;
            std::string::size_type rawlen = raw.size();

            while(lpos < rawlen) {
                
                // retrive key
                std::string::size_type key_star, key_len;
                rpos = raw.find('\n', lpos);
                key_star = lpos;
                key_len  = rpos - lpos;

                
                // retrive value
                lpos = rpos+1;
                rpos = raw.find('\n', lpos);
                long vallen = std::atol(raw + lpos);
                lpos = rpos+1;
                
                // filling batch
                batch.Put(rocksdb::Slice(raw + key_star, key_len), rocksdb::Slice(raw + lpos, vallen));

                //to next iteration
                lpos += vallen + 1;
            }

            // set and filling buffer
            if(_rdb.mset(batch)) {
                buf.add("OK", 2); 
            } else {
                buf.add("FAIL", 4); 
            }
        }

        virtual ~RequestMset() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
