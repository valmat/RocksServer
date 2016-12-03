/**
 *  IniValue.cpp
 *
 *  Class IniValue designed for extracting values from ini entries
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    /**
     *  Casting to integer types
     */
    IniValue<std::string>::operator IniValue<uint64_t> () const {return std::strtoull(_value.c_str(), nullptr, 10);}
    IniValue<std::string>::operator IniValue<uint32_t> () const {return std::strtoul(_value.c_str(), nullptr, 10);}
    IniValue<std::string>::operator IniValue<uint16_t> () const {return std::strtoul(_value.c_str(), nullptr, 10);}
    IniValue<std::string>::operator IniValue<uint8_t>  () const {return std::strtoul(_value.c_str(), nullptr, 10);}

    IniValue<std::string>::operator IniValue<int64_t> () const {return std::stoll(_value);}
    IniValue<std::string>::operator IniValue<int32_t> () const {return std::stol(_value);}
    IniValue<std::string>::operator IniValue<int16_t> () const {return std::stoi(_value);}
    IniValue<std::string>::operator IniValue<int8_t>  () const {return std::stoi(_value);}

    /**
     *  Cast to a EvLogger::Level
     */
    IniValue<std::string>::operator IniValue<EvLogger::Level> () const
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
    IniValue<std::string>::operator IniValue<bool> () const
    {
        std::string v = _value;
        std::transform(v.begin(), v.end(), v.begin(), (int (*)(int))std::tolower);
        return ("true" == v || "1" == v || "on" == v);
    }
    
    /**
     *  Cast to a const char *
     */
    IniValue<std::string>::operator IniValue<const char *> () const
    {
        return _value.c_str();
    }
}
