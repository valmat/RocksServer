/**
 *  RequestDel.h
 *
 *  Request listener for command "/del"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestDel : public RequestBase
    {
    public:
        RequestDel(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       protocol object
         */
        virtual void run(const EvRequest &request, const Protocol &prot) override
        {
            // Detect if current method is POST
            if( !request.isPost() ) {
                


                //buf.add("Request method should be POST");
                prot.fail();



                return;
            }
            
            auto raw = request.getPostData();
            if(!raw.size()) {
                //buf.add("FAIL", 4);
                prot.fail();
                return;
            }
                
            if( _rdb.del(rocksdb::Slice(raw, raw.size())) ) {
                //buf.add("OK", 2);
                prot.ok();
            } else {
                //buf.add("FAIL", 4);
                prot.fail();

            }
        }

        virtual ~RequestDel() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
