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
        RequestGet(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInGet &in, const ProtocolOut &out) override
        {
            if(!in.check()) {
                out.setFailValue();
                return;
            }

            std::string val = _rdb.get(in.key());

            if(!_rdb.status()) {
                out.setFailValue();
            } else {
                out.setValue(val);
            }
        }

        virtual ~RequestGet() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
