/**
 *  RequestDel.h
 *
 *  Request listener for command "/del"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestDel : public RequestBase<ProtocolInPost, ProtocolOut>
    {
    public:
        RequestDel(RocksDBWrapper &rdb) : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInPost &in, const ProtocolOut &out) override
        {
            // Detect if current method is POST
            if( !in.check(out) ) {
                return;
            }
            
            if( db.del(in.key()) ) {
                out.ok();
            } else {
                out.fail();
                EvLogger::writeLog(db.getStatus().data());
            }
        }

        virtual ~RequestDel() {}
    private:
        RocksDBWrapper& db;
    };

}
