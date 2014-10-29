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

        /**
         *  Copy constructor
         */
        PostData(const PostData &that) : _len(that._len), _data(that._data) {}

        /**
         *  Move constructor
         */
        //PostData(PostData &&that) : _len(std::move(that._len)), _data(std::move(that._data)) {}

        /**
         *  Move assignment
         */
        PostData &operator=(PostData &&that)// = default;
        {
            if(this != &that) {
                
                //_len  = std::move(that._len);
                //_data = std::move(that._data);

                _len  = that._len;
                _data = that._data;


                that._data = nullptr;
                that._len = 0;
            }
            //std::cout << "Move assignment" << std::endl;
            //std::cout << "(" << std::endl;
            //std::cout << "data:\t"       << (size_t)_data << std::endl;
            //std::cout << "that._data:\t" << (size_t)that._data << std::endl;
            //std::cout << ")" << std::endl;
            return *this;
        }

        /**
         *  Copy assignment
         */
        PostData &operator=(const PostData &that)// = default;
        {
            if(this != &that) {
                _len  = that._len;
                _data = that._data;
            }
            //std::cout << "Copy assignment" << std::endl;
            return *this;
        }

        ~PostData()
        {
            //if(_data) delete [] _data;
            //std::cout << "data: " << (size_t)_data << std::endl;
            if(_data) {
                //std::cout << "will be deleted" << std::endl;
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
         *  Cast to a std::string
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
        //std::shared_ptr<const char> _data;

        //const char sep = '\n';
    };

}
