/**
 *  EvResponse.h
 *
 *  This is a simple wrapper around evbuffer_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    EvResponse::EvResponse(evhttp_request *req) : _evb(evbuffer_new()), _req(req) {}

    EvResponse::~EvResponse()
    {
        send();
        // Free memory
        if(_evb) evbuffer_free(_evb);
    }

    /**
     *  add formated string to buffer
     *  @param       formated string
     */
    const EvResponse& EvResponse::add_printf(const char *fmt, ...) const
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
    const EvResponse& EvResponse::add(const char *data, size_t datlen) const
    {
        evbuffer_add(_evb, data, datlen);
        return *this;
    }

    const EvResponse& EvResponse::add(const std::string *pstr) const
    {
        evbuffer_add_reference(_evb, pstr->c_str(), pstr->size(), nullptr, nullptr);
        return *this;
    }

    /**
     *  Send buffer as server reply
     *  @param       event buffer wrapper
     */
    void EvResponse::send() const
    {
        if(!_reply_sent && _evb) {
            evhttp_send_reply(_req, HTTP_OK, "OK", _evb);
            _reply_sent = true;
        }
    }

}
