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
        PostData(const PostData &that) : _len(that._len), _data(that._data) 
        {
            std::cout << "\x1b[1;32m Copy constructor \x1b[0m" << std::endl;
        }

        /**
         *  Move constructor
         */
        PostData(PostData &&that) : _len(std::move(that._len)), _data(std::move(that._data)) 
        {
            that._data = nullptr;
            std::cout << "\x1b[1;32m Move constructor \x1b[0m" << std::endl;
        }

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
            std::cout << "\x1b[1;32m Move assignment \x1b[0m" << std::endl;
            std::cout << "(" << std::endl;
            std::cout << "\tdata:\t"       << (size_t)_data << std::endl;
            std::cout << "\tthat._data:\t" << (size_t)that._data << std::endl;
            std::cout << ")" << std::endl << std::endl;
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
            std::cout << "\x1b[1;32m Copy assignment \x1b[0m" << std::endl;
            return *this;
        }

        ~PostData()
        {
            //if(_data) delete [] _data;
            // \x1b[1;34;47m" << npos << "\t" << lpos << "\x1b[0m"
            std::cout << "\x1b[1;34;47m data: " << (size_t)_data << "\x1b[0m"<< std::endl;
            if(_data) {
                std::cout << "\x1b[1;31;47m" << std::string(_data, _len) << "\x1b[0m"<< std::endl;
                delete [] _data;
                _data = nullptr;
                std::cout << "\x1b[1;31;47m will be deleted" << std::endl << "\x1b[0m"<< std::endl;
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
