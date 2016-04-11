/**
 *
 *  Restore RocksDB database from backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <iostream>
#include <unistd.h>  // getopt

// RocksDB
//#include "rocksdb/db.h"
#include "rocksdb/utilities/backupable_db.h"

// RocksDB wrapper
#include "include/rocks/BackupEngine.h"

#include <time.h>


#define HELP_EXIT()  print_help(*argv); return 0;

void print_help(const char *script_name)
{
    std::cout << "usege: " << script_name << " [-i<backup index>] [-l] [-f<backup dir>] [-t<database dir>]";
    std::cout << std::endl << "exemple: " << script_name << " -f/var/rocksserver/backup -t/var/rocksserver/db";
    std::cout << std::endl << "exemple: " << script_name << " -f/var/rocksserver/backup -t/var/rocksserver/db -i5";
    std::cout << std::endl << "exemple: " << script_name << " -l -f/var/rocksserver/backup";
    std::cout << std::endl << "Restore RocksDB backup files to database";
    std::cout << std::endl << "";
    std::cout << std::endl << "-f \tbackup dir";
    std::cout << std::endl << "-t \tdatabase dir";
    std::cout << std::endl << "-i \tbackups index (latest by default)";
    std::cout << std::endl << "-l \tshow backups list";
    std::cout << std::endl << "-h \tprint current help and exit";
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    /*
     * Get options
     */
    char copt=0;
    unsigned optcnt = 0;
    std::string backup_dir, database_dir;
    opterr=0;
    bool show_list = false;
    unsigned index = 0;
    while ( (copt = getopt(argc,argv,"f::t::i::lh")) != -1) {
        ++optcnt;
        switch (copt){
            case 'h':
                HELP_EXIT();
            break;
            case 'f':
                if(optarg) {
                    backup_dir = optarg;
                }
            break;
            case 't':
                if(optarg) {
                    database_dir = optarg;
                }
            break;
            case 'l':
                show_list = true;
            break;
            case 'i':
                if(optarg) {
                    index = std::strtoul(optarg, nullptr, 10);
                }
            break;
            case '?':
                HELP_EXIT();
            break;
        };
    };
    if(optcnt < 1) {
        HELP_EXIT();
    }

    if(show_list && !backup_dir.empty()) {
        auto backup_info = RocksServer::BackupEngine(backup_dir).backupInfo();
        
        std::cout << "found "  << backup_info.size() << " backups:" << std::endl;

        struct tm * dt;
        char buffer [26];
        for(auto &inf: backup_info) {
            dt = localtime(&inf.timestamp);
            strftime(buffer, sizeof(buffer), "%d.%m.%Y %T %z", dt);

            std::cout << "\nid: "      <<  inf.backup_id << std::endl;
            std::cout << "timestamp: " <<  inf.timestamp << std::endl;
            std::cout << "time: "      <<  buffer << std::endl;
            std::cout << "size: "      <<  inf.size << std::endl;
        }
        return 0;
    }

    if(backup_dir.empty() || database_dir.empty()) {
        HELP_EXIT();
    }

    rocksdb::Status status;

    if(index) {
        status = RocksServer::BackupEngine(backup_dir).restoreBackup(database_dir, index);
    } else {
        status = RocksServer::BackupEngine(backup_dir).restoreBackup(database_dir);
    }

    // Show backup status
    std::cout << status.ToString() << std::endl;
    
    return 0;
}