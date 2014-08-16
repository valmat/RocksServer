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
        EvServer(const char *addr = "127.0.0.1", unsigned short port = 5577) : 
            _base(event_base_new())
        {
            if (!_base) {
                std::cerr << "Couldn't create an event_base." << std::endl;
                return;
            }

            // Create a new evhttp object.
            _http = evhttp_new(_base);

            if (!_http) {
                std::cerr << "Couldn't create evhttp." << std::endl;
                return;
            }

            if (evhttp_bind_socket(_http, addr, port) == -1) {
                std::cerr << "Couldn't bind to host " << addr << " port " << port << "." << std::endl;
                return;
            }

            // Print versions
            std::cout << "Libevent version is " << event_get_version() << " ( " << event_get_version_number() << " ) " << std::endl;
            std::cout << "The server is successfully running on " << addr << " port " << port << "." << std::endl;

            // Set signal handlers to safely shut down.
            auto sig_cb = [](evutil_socket_t fd, short what, void *arg) {
                    event_base *_base = (event_base*)arg;
                    event_base_loopexit(_base, NULL);
            };
            event_add(evsignal_new(_base, SIGINT, sig_cb, _base), NULL);    // handler for Ctrl+C
            event_add(evsignal_new(_base, SIGTERM, sig_cb, _base), NULL);   // handler for command `kill <pid>`
     
        }

        ~EvServer()
        {
            if(_http) evhttp_free(_http);
            if(_base) event_base_free(_base);
            for(auto &pReq: _reqList) {
                delete pReq;
            }
            std::cout << "Server closed" << std::endl;
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
         *  Start the event loop
         */
        bool dispatch()
        {
            return (event_base_dispatch(_base) != -1);
        }

        /**
         *  Cast to a bool 
         *  For check if server successfully created
         */
        operator bool () const
        {
            return _http;
        }

        /**
         *  Bind request listener
         *  @param  path       path to listen
         *  @param  req        listener
         */
        void onRequest(const char *path, RequestBase *pReq)
        {
            // Store pointer in scope
            _reqList.push_front(pReq);
            
            evhttp_set_cb(_http, path, [] (evhttp_request *http_req, void *cb_arg) {

                EvBuffer buf;
                if (!buf) return; 
                EvRequest request(http_req);
                
                RequestBase *pReq = reinterpret_cast<RequestBase *>(cb_arg);

                pReq->run(request, buf);
                request.send(buf);

            }, pReq) ;

        }

        /**
         *  Bind request listener
         *  @param  paths       paths to listen
         *  @param  req        listener
         */
        void onRequest(std::initializer_list<const char *> paths, RequestBase *pReq)
        {
            for(auto &path: paths) {
                onRequest(path, pReq);
            }
        }

    private:

        evhttp *_http = nullptr;
        
        // event_base - structure to hold information and state for a Libevent dispatch loop
        event_base *_base;

        // The container for storing a requests listeners
        std::forward_list<RequestBase *> _reqList;
        
    };

}
