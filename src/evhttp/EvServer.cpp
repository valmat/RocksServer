/**
 *  EvServer.cpp
 *
 *  This is a simple wrapper around evhttp_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    /**
     *  Constructor
     *  @param  addr       IP addres, wich server will listen
     *  @param  port       port, wich server will listen
     */
     EvServer::EvServer(const std::string& addr, unsigned short port) : 
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

        if (evhttp_bind_socket(_http, addr.c_str(), port) == -1) {
            std::cerr << "Couldn't bind to host " << addr << " port " << port << "." << std::endl;
            evhttp_free(_http);
            _http = nullptr;
            return;
        }

        // Print versions
        std::cerr << "Libevent version is " << event_get_version() << " ( " << event_get_version_number() << " ) " << std::endl;
        std::cerr << "The server is successfully running on " << addr << " port " << port << "." << std::endl;

        // Set signal handlers to safely shut down.
        auto sig_cb = [](evutil_socket_t fd, short what, void *arg) {
                event_base *_base = (event_base*)arg;
                event_base_loopexit(_base, nullptr);
        };
        event_add(_sigint = evsignal_new(_base, SIGINT, sig_cb, _base), nullptr);    // handler for Ctrl+C
        event_add(_sigterm = evsignal_new(_base, SIGTERM, sig_cb, _base), nullptr);  // handler for command `kill <pid>`
    }

    EvServer::~EvServer()
    {
        // free event handler for SIGINT signal
        if(_sigint) event_free(_sigint);
        // free  event handler for SIGTERM signal
        if(_sigterm) event_free(_sigterm);

        if(_http) evhttp_free(_http);
        if(_base) event_base_free(_base);

        // freeing libevent global variables
        libevent_global_shutdown();
        std::cerr << "Server closed" << std::endl;
    }

    /**
     *  Set server options
     *  @param options EvServerOptions struct
     */
    void EvServer::setOptions(const EvServerOptions &options) const
    {
        if(!options.content_type.empty()) {
            evhttp_set_default_content_type (_http, options.content_type.c_str());
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
    bool EvServer::dispatch()
    {
        return (event_base_dispatch(_base) != -1);
    }

    /**
     *  Bind request listener
     *  @param  path       path to listen
     *  @param  req        listener
     */
    EvServer& EvServer::bind(const char *path, std::unique_ptr<RequestSuperBase> &&pReq)
    {
        evhttp_set_cb(_http, path, [] (evhttp_request *http_req, void *cb_arg) {

            EvRequest request(http_req);
            EvResponse response(http_req);

            if (!response) {
                evhttp_send_error(http_req, 500, "Buffer not allocated");
                return;
            }
            
            reinterpret_cast<RequestSuperBase *>(cb_arg)->run(request, response);
        }, pReq.get()) ;

        // Store a pointer in the scope
        _reqList.push_front(std::move(pReq));
        return *this;
    }

}
