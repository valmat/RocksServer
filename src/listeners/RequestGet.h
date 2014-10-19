/**
 *  RequestGet.h
 *
 *  Request listener for command "get"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestGet : public RequestBase
    {
    public:
        RequestGet(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       protocol object
         */
        virtual void run(const EvRequest &request, const Protocol &prot) override
        {
            std::string uri = request.getUri();
            const std::string::size_type pathlen = uri.find('?');   // length of "/get"
            
            std::string::size_type len = uri.size();
            std::string key;

            if(len-1 <= pathlen) {
                prot.setFailValue();
                return;
            }
                
            key = uri.substr(pathlen + 1, len - pathlen - 1);
            std::string val = _rdb.get(key);

            if(!_rdb.status()) {
                prot.setFailValue();
            } else {
                prot.setValue(val);
            }
        }

        virtual ~RequestGet() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
