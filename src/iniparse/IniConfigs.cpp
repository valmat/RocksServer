/**
 *  IniConfigs.cpp
 *
 *  This is a simplest ini config parser
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    /**
     *  Constructor
     *  @param      configuration file name
     */
    IniConfigs::IniConfigs(const char* file_name)
    {
        std::ifstream file(file_name);
        
        if(!file)
        {
            _isValid = false;
            return;
        }
        
        std::string str;
        for(unsigned line = 1; std::getline(file, str); line++) {
            
            // remove comment
            size_t pos = str.find(';');
            if( std::string::npos > pos ) {
                str = str.substr(  0, pos );
            }
            
            // trim
            trim(str);
            
            if( str.empty() ) {
                continue;
            }
            
            // parse key and value
            pos = str.find('=');
            if( std::string::npos > pos ) {
                std::string key = str.substr(  0, pos );
                std::string value = str.substr( pos+1, str.size() );
                trim(key);
                trim(value);
                
                _map[key] = value;
                
            } else {
                std::cerr << "IniConfigs: syntax error in line: " << line << " [file: " << file_name << "]" << std::endl;
            }
        }
        file.close();
    }

    /**
     *  Trim spaces
     *  @param  strig
     */
    void IniConfigs::trim(std::string &str) const
    {
        // trim from start
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        // trim from end
        str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
    }

}
