/**
 *  EvBuffer.h
 *
 *  This is a simple wrapper around evbuffer_* functions
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class EvBuffer
    {
    public:

        EvBuffer() : _evb(evbuffer_new()) {}

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
        int add_printf(const char *fmt, ...) const
        {
            int res = -1;
            va_list ap;

            va_start(ap, fmt);
            res = evbuffer_add_vprintf(_evb, fmt, ap);
            va_end(ap);

            return (res);
        }

        /**
         *  add string to buffer
         *  @param      string
         *  @param      string lenght
         */
        int add(const char *data, size_t datlen) const
        {
            return evbuffer_add(_evb, data, datlen);
        }

        /**
         *  add string to buffer
         *  @param      string
         *  @param      string lenght
         */
        int add(const std::string &str) const
        {
            return evbuffer_add(_evb, str.c_str(), str.size());
        }

        /**
         *  Cast to a evbuffer pointer
         *  @return evbuffer*
         */
        operator evbuffer* () const
        {
            return _evb;
        }

        ~EvBuffer()
        {
            // Free memory
            if(_evb) evbuffer_free(_evb);
        }

    private:
         evbuffer *_evb;
        
    };

}
