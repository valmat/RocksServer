/**
 *  IniConfigs.h
 *
 *  This is a simplest ini config parser
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

namespace RocksServer {

    class IniConfigs
    {
    public:
        /**
         *  Constructor
         *  @param      configuration file name
         */
        IniConfigs(const char* file_name)
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
                
                if( !str.size() ) {
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
         *  Check file successfully opened
         *  @param      config name
         *  @param      config default value
         *  @param      config value string must be not empty
         */
        template <typename T>
        IniValue<T> get(const std::string &name, const T &value, bool noEmpty = true) const
        {
            // if key not exist
            if(_map.find(name) == _map.cend()) {
                return IniValue<T>(value);
            }

            // check if config value string is empty
            if(noEmpty && !_map.at(name).size()) {
                return IniValue<T>(value);
            }
            
            return IniValue<std::string>( _map.at(name) );
        }
        
        ~IniConfigs() {}
        
        /**
         *  Check file successfully opened
         */
        operator bool () const
        {
            return _isValid;
        }

    private:
        /**
         *  trim spaces
         *  @param  strig
         */
        void trim(std::string &str) const
        {
            // trim from start
            str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            // trim from end
            str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
        }
        
        bool _isValid = true;
        std::unordered_map<std::string, std::string> _map;
    };

}
