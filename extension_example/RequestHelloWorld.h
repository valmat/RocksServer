/**
 *  RequestHelloWorld.h
 *
 *  Example request listener
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestHelloWorld : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
    public:
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override
        {
            out.setStr("Hello world!");
        }

        virtual ~RequestHelloWorld() 
        {
            std::cout << "~RequestHelloWorld()" << std::endl;
        }
    };

}
