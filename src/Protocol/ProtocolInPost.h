 /**
 *  ProtocolInPost.h
 *
 *  Bridge to work with  a request.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class ProtocolInPost
    {
    public:
        ProtocolInPost(const EvRequest &r) :request(r), raw(request.getPostData()) {}

        // Check query
        bool check(const ProtocolOut &out) const
        {
            // Detect if current method is POST
            if(!request.isPost()) {
                EvLogger::writeLog("Request method should be POST");
                out.fail();
                return false;
            }

            // Check if POST data is empty
            if(!raw.size()) {
                EvLogger::writeLog("Request POST data is empty");
                out.fail();
                return false;
            }

            return true;
        }

        // retrive key
        rocksdb::Slice key() const
        {
            return rocksdb::Slice((const char *)raw, raw.size());
        }

        // retrive key and value
        std::pair<rocksdb::Slice, rocksdb::Slice> pair() const
        {
            std::string::size_type lpos = 0;
            std::string::size_type rpos = raw.find('\n');
            rocksdb::Slice key(raw, rpos);
            
            lpos = rpos+1;
            rpos = raw.find('\n', lpos);
            auto vallen = std::atoll((const char *)raw + lpos);
            lpos = rpos+1;
            rocksdb::Slice value((const char *)raw + lpos, vallen);

            return std::make_pair(key, value);
        }

    protected:
        const EvRequest &request;
        const PostData raw;
    };

}
