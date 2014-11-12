/**
 *  IniValue.h
 *
 *  Class IniValue designed for extracting values from ini entries
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

#include <cstdint>

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
         *  Cast to an object of type T
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
         *  Casting to integer types
         */
        operator IniValue<uint64_t> () const {return std::strtoull(_value.c_str(), nullptr, 10);}
        operator IniValue<uint32_t> () const {return std::strtoul(_value.c_str(), nullptr, 10);}
        operator IniValue<uint16_t> () const {return std::strtoul(_value.c_str(), nullptr, 10);}
        operator IniValue<uint8_t>  () const {return std::strtoul(_value.c_str(), nullptr, 10);}

        operator IniValue<int64_t> () const {return std::stoll(_value);}
        operator IniValue<int32_t> () const {return std::stol(_value);}
        operator IniValue<int16_t> () const {return std::stoi(_value);}
        operator IniValue<int8_t>  () const {return std::stoi(_value);}

        /**
         *  Cast to a EvLogger::Level
         */
        operator IniValue<EvLogger::Level> () const
        {
            if("debug" == _value) return EvLogger::Level::debug;
            if("msg"   == _value) return EvLogger::Level::msg;
            if("warn"  == _value) return EvLogger::Level::warn;
            if("error" == _value) return EvLogger::Level::error;
            if("fatal" == _value) return EvLogger::Level::fatal;
            
            return EvLogger::Level::none;
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
         *  Cast to a const char *
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
