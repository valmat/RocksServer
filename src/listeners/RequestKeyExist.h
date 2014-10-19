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
         *  @param       event request object
         *  @param       protocol object
         */
        virtual void run(const EvRequest &request, const Protocol &prot) override
        {
            std::string uri = request.getUri();
            const std::string::size_type pathlen = uri.find('?');   // length of "/exist"
            std::string::size_type len = uri.size();

            if(len-1 <= pathlen) {
                //buf.add("FAIL\n", 5);
                prot.fail();
                return;
            }
                
            std::string value;
            bool value_found;
            bool result = _rdb.keyExist(rocksdb::Slice(uri.c_str() + pathlen + 1, len - pathlen - 1), value, value_found);

            // Add buffer
            if(result) {
                //buf.add("OK\n", 3);
                prot.ok();

                if(value_found) {
                    //buf.add_printf("%lu\n%s", value.size(), value.c_str());
                    prot.setValue(val);
                } else {
                    //buf.add("-1\n", 3);
                    prot.setFailValue();
                }

            } else {
                //buf.add("FAIL\n", 5);
                prot.fail();
            }
        }

        virtual ~RequestKeyExist() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
