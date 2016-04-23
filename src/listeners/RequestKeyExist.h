/**
 *  RequestKeyExist.h
 *
 *  Request listener for command "exit" (check if key exist)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestKeyExist : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestKeyExist(RocksDBWrapper &rdb) : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) override
        {
            if(!in.check()) {
                out.fail();
                return;
            }
                
            std::string value;
            if(db.keyExist(in.key(), value)) {
                out.ok();
                out.setValue(value);
            } else {
                out.fail();
            }
        }

        virtual ~RequestKeyExist() {}
    private:
        RocksDBWrapper& db;
    };

}
