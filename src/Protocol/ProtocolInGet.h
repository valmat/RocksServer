 /**
 *  ProtocolInGet.h
 *
 *  Bridge to work with  a request.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class ProtocolInGet
    {
    public:
        ProtocolInGet(const EvRequest &r) : 
            request(r), 
            uri(r.getUri()), 
            pathlen(uri.find('?')),
            len(uri.size())
        {}

        //ProtocolInGet(EvRequest &&r) :request(std::move(r)) {}

        // Detect if current query is valid
        bool check() const
        {
            return (len-1 > pathlen);
        }

        rocksdb::Slice key() const
        {
            return rocksdb::Slice(uri.data() + pathlen + 1, len - pathlen - 1);
        }

        ProtocolInGetIterator begin() const
        {
            return ProtocolInGetIterator(uri, len, pathlen+1);
        }

        ProtocolInGetIterator end() const
        {
            return ProtocolInGetIterator();
        }

        

        ~ProtocolInGet() {}

    protected:
        const EvRequest &request;
        const std::string uri;
        // length of "/path"
        const std::string::size_type pathlen;

        const std::string::size_type len;
    };

}
