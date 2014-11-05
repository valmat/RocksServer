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
            pathlen(uri.find('?'))
        {}

        // Detect if current query is valid
        bool check() const
        {
            return (uri.size()-1 > pathlen);
        }

        rocksdb::Slice key() const
        {
            return rocksdb::Slice(uri.data() + pathlen + 1, uri.size() - pathlen - 1);
        }

        ProtocolInGetIterator begin() const
        {
            return ProtocolInGetIterator(uri, pathlen+1);
        }

        ProtocolInGetIterator end() const
        {
            return ProtocolInGetIterator();
        }

    protected:
        const EvRequest &request;
        const std::string uri;
        // length of "/path"
        const std::string::size_type pathlen;
    };

}
