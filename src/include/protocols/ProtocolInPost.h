 /**
 *  ProtocolInPost.h
 *
 *  Bridge to work with  a request.
 *  For http method POST
 *
 *  If the request http method is the POST 
 *  and you just need to get data such as key or key-value pair, use this class
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
            if(!isPost()) {
                EvLogger::writeLog("Request method should be POST");
                out.fail();
                return false;
            }

            // Check if POST data is empty
            if(isEmpty()) {
                EvLogger::writeLog("Request POST data is empty");
                out.fail();
                return false;
            }

            return true;
        }

        bool isPost() const
        {
            return request.isPost();
        }

        bool isEmpty() const
        {
            return !raw.size();
        }

        // retrive key
        rocksdb::Slice key() const
        {
            return rocksdb::Slice(raw, raw.size());
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
