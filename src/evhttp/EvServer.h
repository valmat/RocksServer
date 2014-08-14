/**
 *  EvServer.h
 *
 *  This is a simple wrapper around evhttp_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class EvServer
    {
    public:
        /**
         *  Constructor
         *  @param  addr       IP addres, wich server will listen
         *  @param  port       port, wich server will listen
         */
        EvServer(const char addr[] = "127.0.0.1", unsigned short port = 5577) : 
            _http(evhttp_start(addr, port))
        {
            std::cout << "The server is successfully running on " << addr << " port " << port << "." << std::endl;
            std::cout << "Libevent version is " << event_get_version() << " ( " << event_get_version_number() << " ) " << std::endl;
        }

        ~EvServer()
        {
            evhttp_free(_http);
            //std::cout << "server closed" << std::endl;
        }

        /**
         *  Set server options
         *  @param options EvServerOptions struct
         */
        void setOptions(const EvServerOptions &options) const
        {
            if(options.content_type) {
                evhttp_set_default_content_type (_http, options.content_type);
            }
            if(options.max_body_size) {
                evhttp_set_max_body_size (_http, options.max_body_size);
            }
            if(options.max_headers_size) {
                evhttp_set_max_headers_size (_http, options.max_headers_size);
            }
            if(options.allowed_methods) {
                // Sets the what HTTP methods are supported in requests accepted by this server, 
                // and passed to user callbacks.
                // If not supported they will generate a "405 Method not allowed" response.
                // By default this includes the following methods: GET, POST, HEAD, PUT, DELETE
                evhttp_set_allowed_methods(_http, options.allowed_methods);
            }
        }

        /**
         *  Check if server successfully created
         */
        bool isValid()
        {
            return _http;
        }

        /**
         *  Bind request listener
         *  @param  path       path to listen
         *  @param  req        listener
         */
        void onRequest(const char *path, RequestBase &&req)
        {
            evhttp_set_cb(_http, path, [] (evhttp_request *http_req, void *cb_arg) {

                EvBuffer buf;
                if (!buf) {
                    return; 
                }
                EvRequest request(http_req);
                RequestBase *pReq = static_cast<RequestBase *>(cb_arg);

                pReq->run(request, buf);
                request.send(buf);

            }, &req) ;

        }

        /**
         *  Bind request listener
         *  @param  paths       paths to listen
         *  @param  req        listener
         */
        void onRequest(std::initializer_list<const char *> paths, RequestBase &&req)
        {
            for(auto &path: paths) {
                onRequest(path, std::move(req));
            }
        }

    private:

        evhttp* _http;
        
    };

}
