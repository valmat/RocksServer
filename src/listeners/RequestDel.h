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
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &request, const EvBuffer &buf) override
        {
            auto raw = request.getPostData();
            if(!raw.size()) {
                buf.add("FAIL", 4);
                return;
            }
                
            if( _rdb.del(rocksdb::Slice(raw, raw.size())) ) {
                buf.add("OK", 2);
            } else {
                buf.add("FAIL", 4);
            }
        }

        virtual ~RequestDel() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
