/**
 *  EvRequest.h
 *
 *  This is a simple wrapper around evhttp_request_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


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
            //evhttp_request_free(_req); <-- not required
        }

        /**
         *  Get request URI string
         */
        std::string getUri() const
        {
            return  evhttp_request_get_uri(_req);
        }

        /**
         *  Get raw post data wrapper
         */
        PostData getPostData() const
        {
            evbuffer *in_evb = evhttp_request_get_input_buffer(_req);
            size_t len = evbuffer_get_length(in_evb);
            char *data = new char[len];
            evbuffer_copyout(in_evb, data, len);

            return PostData(data, len);
            //if(in_evb) evbuffer_free(in_evb);
        }

        /**
         *  Get response code
         */
        int getCode() const
        {
            return evhttp_request_get_response_code(_req);
        }

        /**
         *  Get request method
         *  see: http://www.wangafu.net/~nickm/libevent-2.1/doxygen/html/http_8h.html
         */
        evhttp_cmd_type getMethod() const
        {
            return evhttp_request_get_command(_req);
        }

        /**
         *  Check if current method is POST
         */
        bool isPost() const
        {
            return ( EVHTTP_REQ_POST == getMethod() );
        }

        /**
         *  Send buffer as server reply
         *  @param       event buffer wrapper
         */
        void send(const EvBuffer &evBuf) const
        {
            return  evhttp_send_reply(_req, HTTP_OK, "OK", evBuf);
        }

    private:
         evhttp_request *_req;
    };

}
