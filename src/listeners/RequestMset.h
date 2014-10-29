/**
 *  RequestMset.h
 *
 *  Request listener for command "mset" (multiset)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestMset : public RequestBase<ProtocolInPostPairs, ProtocolOut>
    {
    public:
        RequestMset(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostPairs &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !in.check(out)) {
                return;
            }
            
            // create a RocksDB write batch
            rocksdb::WriteBatch batch;
            for (auto &it : in) {
                batch.Put(it.first, it.second);
            }

            /*
            auto raw = in.getRawPost();

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
                long vallen = std::atol((const char *)raw + lpos);
                lpos = rpos+1;
                
                // filling batch
                batch.Put(rocksdb::Slice((const char *)raw + key_star, key_len), rocksdb::Slice(raw + lpos, vallen));

                //to next iteration
                lpos += vallen + 1;
            }
            */



            // set and filling buffer
            if(_rdb.mset(batch)) {
                out.ok();
            } else {
                out.fail(); 
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestMset() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
