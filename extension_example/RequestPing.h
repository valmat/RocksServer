/**
 *  RequestPing.h
 *
 *  Example request listener
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    struct RequestPing : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override
        {
            out.setStr("pong");
        }
        
        virtual ~RequestPing() {}
    };
}
