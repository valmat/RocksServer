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
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &, const EvBuffer &) = 0;
    };

}
