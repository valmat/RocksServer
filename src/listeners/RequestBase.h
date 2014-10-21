/**
 *  RequestBase.h
 *
 *  Interface for request listeners
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {


    struct RequestBase
    {
        virtual ~RequestBase() {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolIn &, const ProtocolOut &) = 0;
        
    };

}
