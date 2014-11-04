/**
 *  EvRequest.h
 *
 *  This is a simple wrapper around evhttp_request_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    /**
     *  Get request URI string
     */
    std::string EvRequest::getUri() const
    {
        return  evhttp_request_get_uri(_req);
    }

    /**
     *  Get raw post data wrapper
     */
    PostData EvRequest::getPostData() const
    {
        evbuffer *in_evb = evhttp_request_get_input_buffer(_req);
        size_t len = evbuffer_get_length(in_evb);
        char *data = new char[len];
        evbuffer_copyout(in_evb, data, len);
        return PostData(data, len);
    }

    /**
     *  Get response code
     */
    int EvRequest::getCode() const
    {
        return evhttp_request_get_response_code(_req);
    }

    /**
     *  Get request method
     *  see: http://www.wangafu.net/~nickm/libevent-2.1/doxygen/html/http_8h.html
     */
    evhttp_cmd_type EvRequest::getMethod() const
    {
        return evhttp_request_get_command(_req);
    }

    /**
     *  Check if current method is POST
     */
    bool EvRequest::isPost() const
    {
        return ( EVHTTP_REQ_POST == getMethod() );
    }

}
