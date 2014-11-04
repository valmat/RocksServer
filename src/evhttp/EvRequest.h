/**
 *  EvRequest.h
 *
 *  This is a simple wrapper around evhttp_request_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

// Forward declaration
struct evhttp_request;

namespace RocksServer {

    class EvRequest
    {
    public:
        /**
         *  Constructor
         *  @param      evhttp_request pointer
         */
        EvRequest(evhttp_request *req) : _req(req) {}

        ~EvRequest() 
        {
            //if(_req) evhttp_request_free(_req); //<-- not required, 
                                                  // because it was called in the function evhttp_send (through evhttp_send_done)
        }

        /**
         *  Get request URI string
         */
        std::string getUri() const;

        /**
         *  Get request query string
         */
        std::string getUriQuery() const
        {
            std::string uri = getUri();
            std::string::size_type pathlen = uri.find('?');   // length of "/path"
            std::string::size_type len = uri.size();
            
            if(len-1 <= pathlen) {
                return "";
            }
            return uri.substr(pathlen + 1, len - pathlen - 1);
        }

        /**
         *  Get raw post data wrapper
         */
        PostData getPostData() const;

        /**
         *  Get response code
         */
        int getCode() const;

        /**
         *  Get request method
         *  see: http://www.wangafu.net/~nickm/libevent-2.1/doxygen/html/http_8h.html
         */
        evhttp_cmd_type getMethod() const;

        /**
         *  Check if current method is POST
         */
        bool isPost() const;

        /**
         *  Cast to a evhttp_request pointer
         *  @return evhttp_request*
         */
        operator evhttp_request* () const
        {
            return _req;
        }

    private:
        evhttp_request *_req;
    };

}
