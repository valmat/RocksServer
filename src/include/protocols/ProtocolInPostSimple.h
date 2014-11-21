 /**
 *  ProtocolInPostSimple.h
 *
 *  Bridge to work with  a request.
 *  For http method POST
 *
 *  If the request http method is the POST 
 *  you don't need retrieve any data from http request, use this class
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class ProtocolInPostSimple
    {
    public:
        ProtocolInPostSimple(const EvRequest &r) :request(r) {}

        // Detect if current method is POST
        bool check(const ProtocolOut &out) const
        {
            if(!request.isPost()) {
                EvLogger::writeLog("Request method should be POST");
                out.fail();
                return false;
            }
            return true;
        }

    protected:
        const EvRequest &request;
    };

}
