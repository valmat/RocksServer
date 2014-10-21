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


        void fail() const
        {
            //request.add("FAIL\n", 5);
        }

        ~ProtocolIn() {}
    private:
        const EvRequest &request;
    };

}
