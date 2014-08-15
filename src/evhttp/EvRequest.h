/**
 *  EvRequest.h
 *
 *  This is a simple wrapper around EvRequest_* functions
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
            struct evbuffer *in_evb = evhttp_request_get_input_buffer(_req);
            size_t len = evbuffer_get_length(in_evb);
            char *data = new char[len];
            evbuffer_copyout(in_evb, data, len);

            return PostData(data, len);
        }

        /**
         *  Send buffer as server reply
         *  @param       event buffer wrapper
         */
        void send(const EvBuffer &evBuf) const
        {
            return  evhttp_send_reply(_req, HTTP_OK, "OK", evBuf);
        }

        ~EvRequest() {}

    private:
         evhttp_request *_req;
    };

}
