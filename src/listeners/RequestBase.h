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
         *  @param       event request object
         *  @param       protocol object
         */
        virtual void run(const EvRequest &, const ProtocolOut &) = 0;
        
    };

}
