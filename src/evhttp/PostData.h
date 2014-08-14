/**
 *  PostData.h
 *
 *  Raw post data wrapper
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class PostData
    {
    public:
        /**
         *  Constructor
         * 
         *  @param  data       raw string of post data
         *  @param  len        string length
         */
        PostData(const char *data, size_t len) : _len(len), _data(data) {}

        ~PostData()
        {
            delete _data;
        }

        /**
         *  Cast to a std::string
         */
        std::string toString()
        {
            return std::string(_data, _len);
        }

        /**
         *  Cast to a const char *
         *  @return evbuffer*
         */
        operator const char * () const
        {
            return _data;
        }

        /**
         *  Cast to a std::string
         */
        size_t size()
        {
            return _len;
        }

        /**
         *  Find character position at data string
         *  @param  s       searching character
         *  @param  start   start position
         */
        size_t find(char s, size_t start = 0)
        {
            size_t pos = start;
            for(; _data[pos] != s && pos < _len; pos++);
            return (pos < _len) ? pos : std::string::npos;
        }
    //private:


    private:
        size_t _len;
        const char *_data;
        const char sep = '\n';
    };

}
