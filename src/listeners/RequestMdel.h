/**
 *  RequestMdel.h
 *
 *  Request listener for command "mset" (multiset)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestMdel : public RequestBase<ProtocolInPostKeys, ProtocolOut>
    {
    public:
        RequestMdel(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPostKeys &in, const ProtocolOut &out) override
        {
            
            //std::cout << "\t  RequestMdel :" << in.raw.size()  << std::endl;
            //std::cout << "\t          :\n" << in.raw  << std::endl;


            // Detect if current method is POST
            //if( !in.checkPost(out) || !in.checkPostSize(out) ) {
            if(!in.check(out)) {
                return;
            }
            
            // create a RocksDB delete batch
            rocksdb::WriteBatch batch;

            /*
            auto raw = in.getRawPost();

            std::string::size_type lpos = 0;
            std::string::size_type rpos = raw.find('\n');
            std::string::size_type len  = raw.size();

            // filling batch
            while(rpos < std::string::npos) {
                batch.Delete(rocksdb::Slice(raw+lpos, rpos-lpos));
                lpos = rpos+1;
                rpos = raw.find('\n', lpos);
            }
            batch.Delete(rocksdb::Slice(raw+lpos, len-lpos));
            */

            //int i = 0;
            for (auto &it : in) {
                //std::cout << i++ << "\t" << it.ToString() << std::endl;
                batch.Delete(it);
            }



            if(_rdb.mset(batch)) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestMdel() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
