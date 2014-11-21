/**
 *  EvResponse.h
 *
 *  This is a simple wrapper around evbuffer_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

// Forward declaration
struct evbuffer;
struct evhttp_request;

namespace RocksServer {

    class EvResponse
    {
    public:

        EvResponse(evhttp_request *req);

        ~EvResponse();

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
        const EvResponse& add_printf(const char *fmt, ...) const;

        /**
         *  add string to buffer
         *  @param      string
         *  @param      string lenght
         */
        const EvResponse& add(const char *data, size_t datlen) const;

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

        const EvResponse& add(const std::string *pstr) const;

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
        void send() const;

    private:
        evbuffer *_evb;
        evhttp_request *_req;
        mutable bool _reply_sent = false;
        
    };

    //typedef EvResponse EvBuffer;

}
