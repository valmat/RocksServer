/**
 *  RequestGet.h
 *
 *  Request listener for command "get"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestGet : public RequestBase<ProtocolInGet, ProtocolOut>
    {
    public:
        RequestGet(RocksDBWrapper &rdb) : db(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) override
        {
            // Check if any data transfered
            if(!in.check()) {
                out.setFailValue();
                return;
            }

            std::string val = db.get(in.key());

            if(!db.status()) {
                out.setFailValue();
            } else {
                out.setValue(val);
            }
        }

        virtual ~RequestGet() {}
    private:
        RocksDBWrapper& db;
    };

}
