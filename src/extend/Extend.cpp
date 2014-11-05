/**
 *
 *  Extend.cpp
 *  Extend RocksServer
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"
#include <dirent.h>
 
namespace RocksServer {

    void Extend::load(EvServer &server, const RocksDBWrapper &rdb, const IniConfigs &cfg, const DefaultConfigs &dfCfg)
    {
        // Read directory for load extentions
        std::string extdir = cfg.get("extdir", dfCfg.extdir);
        DIR *dp;
        if( nullptr == (dp = opendir(extdir.c_str())) ) {
            std::cout << "Error(" << errno << ") opening " << extdir << std::endl;
            return;
        }

        for(struct dirent *dirp = readdir(dp); dirp;  dirp = readdir(dp)) {
            // Compare file extention
            if ( strncmp ( dirp->d_name + strlen(dirp->d_name)-3, ".so", 3 ) != 0) {
              continue;
            }
            
            std::string plug_file = extdir + "/" + dirp->d_name;
            std::cout << "Load extension " << dirp->d_name << std::endl;

            handles.emplace_front(std::move(plug_file), server, rdb, cfg);
        }
        closedir(dp);
    }

}