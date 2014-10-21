/**
 *  RequestKeyExist.h
 *
 *  Request listener for command "exit" (check if key exist)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestKeyExist : public RequestBase
    {
    public:
        RequestKeyExist(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolIn &in, const ProtocolOut &out) override
        {
            std::string uri = request.getUri();
            const std::string::size_type pathlen = uri.find('?');   // length of "/exist"
            std::string::size_type len = uri.size();

            if(len-1 <= pathlen) {
                out.fail();
                return;
            }
                
            std::string value;
            bool value_found;
            bool result = _rdb.keyExist(rocksdb::Slice(uri.c_str() + pathlen + 1, len - pathlen - 1), value, value_found);

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
