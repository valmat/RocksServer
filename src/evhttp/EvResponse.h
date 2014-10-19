/**
 *  EvResponse.h
 *
 *  This is a simple wrapper around evbuffer_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class EvResponse
    {
    public:

        EvResponse(evhttp_request *req) : _evb(evbuffer_new()), _req(req) {}

        ~EvResponse()
        {
            send();
            // Free memory
            if(_evb) evbuffer_free(_evb);
        }

        /**
         *  Check if buffer successfully created
         */
        operator bool () const
        {
            return _evb;
        }

        /**
         *  add formated string to buffer
         *  @param       formated string
         */
        const EvResponse& add_printf(const char *fmt, ...) const
        {
            va_list ap;
            va_start(ap, fmt);
            evbuffer_add_vprintf(_evb, fmt, ap);
            va_end(ap);

            return *this;
        }

        /**
         *  add string to buffer
         *  @param      string
         *  @param      string lenght
         */
        const EvResponse& add(const char *data, size_t datlen) const
        {
            evbuffer_add(_evb, data, datlen);
            return *this;
        }

        /**
         *  add string to buffer
         *  @param      string
         */
        const EvResponse& add(const char *data) const
        {
            return add(data, strlen(data));
        }

        /**
         *  add string to buffer
         *  @param      string
         *  @param      string lenght
         */
        const EvResponse& add(const std::string &str) const
        {
            return add(str.c_str(), str.size());
        }

        const EvResponse& add(size_t val) const
        {
            add_printf("%lu", val);
            return *this;
        }

        const EvResponse& endl() const
        {
            add("\n", 1);
            return *this;
        }

        /**
         *  Cast to a evbuffer pointer
         *  @return evbuffer*
         */
        operator evbuffer* () const
        {
            return _evb;
        }

        /**
         *  Send buffer as server reply
         *  @param       event buffer wrapper
         */
        void send() const
        {
            if(!_reply_sent && _evb) {
                evhttp_send_reply(_req, HTTP_OK, "OK", _evb);
                _reply_sent = true;
            }
        }

    private:
        evbuffer *_evb;
        evhttp_request *_req;
        mutable bool _reply_sent = false;
        
    };

    typedef EvResponse EvBuffer;

}
