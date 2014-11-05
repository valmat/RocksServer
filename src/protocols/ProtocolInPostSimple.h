 /**
 *  ProtocolInPostSimple.h
 *
 *  Bridge to work with  a request.
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
