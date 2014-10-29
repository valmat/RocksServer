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
        RequestKeyExist(RocksDBWrapper &rdb) : _rdb(rdb) {}

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
            bool value_found;
            bool result = _rdb.keyExist(in.key(), value, value_found);

            if(result) {
                out.ok();
                if(value_found) {
                    out.setValue(value);
                } else {
                    out.setFailValue();
                }
            } else {
                out.fail();
            }
        }

        virtual ~RequestKeyExist() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
