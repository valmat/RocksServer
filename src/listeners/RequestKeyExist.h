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
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &request, const EvBuffer &buf) override
        {
            std::string uri = request.getUri();
            const std::string::size_type pathlen = uri.find('?');   // length of "/exist"
            std::string::size_type len = uri.size();

            if(len-1 <= pathlen) {
                // Add buffer
                buf.add("FAIL\n", 5);
                return;
            }
                
            std::string value;
            bool value_found;
            bool result = _rdb.keyExist(rocksdb::Slice(uri.c_str() + pathlen + 1, len - pathlen - 1), value, value_found);

            //key = uri.substr(pathlen + 1, len - pathlen - 1);
            
            // Add buffer
            if(result) {
                buf.add("OK\n", 3);

                if(value_found) {
                    buf.add_printf("%lu\n%s", value.size(), value.c_str());
                } else {
                    buf.add("-1\n", 3);
                }

            } else {
                buf.add("FAIL\n", 5);
            }


            //std::cout << "------------RequestKeyExist" << std::endl;
        }

        virtual ~RequestKeyExist() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
