 /**
 *  ProtocolIn.h
 *
 *  Bridge to work with  a request.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class ProtocolIn
    {
    public:
        ProtocolIn(const EvRequest &r) :request(r) {}
        ProtocolIn(EvRequest &&r) :request(std::move(r)) {}

        // Detect if current method is POST
        bool checkPost(const ProtocolOut &out) const
        {
            if(!request.isPost()) {
                EvLogger::writeLog("Request method should be POST");
                out.fail();
                return false;
            }
            return true;
        }

        // Check if POST data is empty
        bool checkPostSize(const ProtocolOut &out) const
        {
            if(!getRawPost().size()) {
                EvLogger::writeLog("Request POST data is empty");
                out.fail();
                return false;
            }
            return true;
        }

        //const PostData& getRawPost() const
        PostData& getRawPost() const
        {
            if(!raw.isValid()) {
                //raw = request.getPostData();
                raw = std::move(request.getPostData());
            }
            return raw;
        }

        std::string getUri() const
        {
            return request.getUri();
        }

    private:
        const EvRequest &request;
        mutable PostData raw;
    };

}
