/**
 *  IniValue.h
 *
 *  Class IniValue designed for extracting values from ini entries
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

namespace RocksServer {

    template <typename T>
    class IniValue
    {
    public:
        /**
         *  Constructors
         *  @param      default value
         */
         IniValue(const T &value) : _value(value) {}
         IniValue(const T &&value) : _value(std::move(value)) {}

        /**
         *  Cast to a object of type T
         */
        operator T () const
        {
            return _value;
        }
        
    private:

        /**
         *  default value
         */
        const T _value;
    };


    template<> 
    class IniValue<std::string>
    {
    public:
        
        /**
         *  Constructors
         *  @param      string value
         */
        IniValue(const std::string &value) : _value(value) {}
        IniValue(const std::string &&value) : _value(std::move(value)) {}

        /**
         *  Cast to a string
         */
        operator std::string () const
        {
            return _value;
        }

        /**
         *  Cast to a size_t
         */
         operator IniValue<size_t> () const
        {
            return std::strtoull(_value.c_str(), nullptr, 10);
        }

        /**
         *  Cast to a size_t
         */
         operator IniValue<unsigned short> () const
        {
            return std::strtoul(_value.c_str(), nullptr, 10);
        }

        /**
         *  Cast to a number
         */
        operator IniValue<int> () const
        {
            return 1;
        }
        
        /**
         *  Cast to a bool
         */
        operator IniValue<bool> () const
        {
            std::string v = _value;
            std::transform(v.begin(), v.end(), v.begin(), (int (*)(int))std::tolower);
            return ("true" == v || "1" == v || "on" == v);
        }
        
        /**
         *  Cast to const char *
         */
        operator IniValue<const char *> () const
        {
            return _value.c_str();
        }
        
    private:
        
        /**
         *  string value
         */
        const std::string _value;
    };

}
