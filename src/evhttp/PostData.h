/**
 *  PostData.h
 *
 *  Raw post data wrapper
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class PostData
    {
    public:
        /**
         *  Constructor
         *  @param  data       raw string of post data
         *  @param  len        string length
         */
        PostData(const char *data, size_t len) : _len(len), _data(data) {}

        /**
         *  Trivial constructor
         */
        PostData() : _len(0), _data(nullptr) {}

        ~PostData()
        {
            if(_data) {
                delete [] _data;
                _data = nullptr;
            }
        }

        /**
         *  Cast to a std::string
         */
        std::string toString() const
        {
            return std::string(_data, _len);
        }

        /**
         *  Cast to a const char *
         */
        operator const char * () const
        {
            return _data;
        }

        /**
         *  Cast to a const char *
         */
        const char * data() const
        {
            return _data;
        }

        /**
         *  Buffer size
         */
        size_t size() const
        {
            return _len;
        }

        /**
         *  Find character position at data string
         *  @param  s       searching character
         *  @param  start   start position
         */
        size_t find(char s, size_t start = 0) const
        {
            size_t pos = start;
            for(; pos < _len && _data[pos] != s ; pos++);
            return (pos < _len) ? pos : std::string::npos;
        }

        /**
         *  Cast to a bool
         */
        bool isValid () const
        {
            return _data;
        }

    private:
        size_t _len;
        const char *_data;
    };

}
