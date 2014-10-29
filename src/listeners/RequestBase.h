/**
 *  RequestBase.h
 *
 *  Interface for request listeners
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {


    struct RequestSuperBase
    {
        virtual ~RequestSuperBase() {}

        /**
         *  Runs request listener
         *  @param       EvRequest object
         *  @param       EvResponse object
         */
        virtual void run(const EvRequest &request, const EvResponse &response) = 0;
    };


    template<typename ProtIn, typename ProtOut>
    struct RequestBase: public RequestSuperBase
    {
        virtual ~RequestBase() {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtIn &, const ProtOut &) = 0;

    private:
        /**
         *  Runs request listener
         *  @param       EvRequest object
         *  @param       EvResponse object
         */
        virtual void run(const EvRequest &request, const EvResponse &response) override
        {
            run(std::move(ProtIn{request}), std::move(ProtOut{response}) );
        }
    };

}
