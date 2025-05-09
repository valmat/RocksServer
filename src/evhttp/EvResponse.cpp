/**
 *  EvResponse.cpp
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

    void EvResponse::set_code(int code, const char* reason) const
    {
        _code = code;
        _reason = reason;
    }

    void EvResponse::set_code(int code) const
    {
        _code = code;
        switch(code) {
            case 400: _reason = "Bad Request"; break;
            case 401: _reason = "Unauthorized"; break;
            case 402: _reason = "Payment Required"; break;
            case 403: _reason = "Forbidden"; break;
            case 404: _reason = "Not Found"; break;
            case 405: _reason = "Method Not Allowed"; break;
            case 406: _reason = "Not Acceptable"; break;
            case 407: _reason = "Proxy Authentication Required"; break;
            case 408: _reason = "Request Timeout"; break;
            case 409: _reason = "Conflict"; break;
            case 410: _reason = "Gone"; break;
            case 411: _reason = "Length Required"; break;
            case 412: _reason = "Precondition Failed"; break;
            case 413: _reason = "Content Too Large"; break;
            case 414: _reason = "URI Too Long"; break;
            case 415: _reason = "Unsupported Media Type"; break;
            case 416: _reason = "Range Not Satisfiable"; break;
            case 417: _reason = "Expectation Failed"; break;
            case 421: _reason = "Misdirected Request"; break;
            case 422: _reason = "Unprocessable Content"; break;
            case 423: _reason = "Locked"; break;
            case 424: _reason = "Failed Dependency"; break;
            case 425: _reason = "Too Early"; break;
            case 426: _reason = "Upgrade Required"; break;
            case 428: _reason = "Precondition Required"; break;
            case 429: _reason = "Too Many Requests"; break;
            case 431: _reason = "Request Header Fields Too Large"; break;
            case 451: _reason = "Unavailable for Legal Reasons"; break;
            case 500: _reason = "Internal Server Error"; break;
            case 501: _reason = "Not Implemented"; break;
            case 502: _reason = "Bad Gateway"; break;
            case 503: _reason = "Service Unavailable"; break;
            case 504: _reason = "Gateway Timeout"; break;
            case 505: _reason = "HTTP Version Not Supported"; break;
            case 506: _reason = "Variant Also Negotiates"; break;
            case 507: _reason = "Insufficient Storage"; break;
            case 508: _reason = "Loop Detected"; break;
            case 511: _reason = "Network Authentication Required"; break;
            default:  _reason = "Unknown Status"; break;
        }
    }    

    /**
     *  Send buffer as server reply
     *  @param       event buffer wrapper
     */
    void EvResponse::send() const
    {
        if(!_reply_sent && _evb) {
            evhttp_send_reply(_req, _code, _reason, _evb);
            _reply_sent = true;
        }
    }

}
