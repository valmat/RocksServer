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

#define HELP_EXIT()  print_help(*argv); return 0;

void print_help(const char *script_name)
{
    std::cout << "usege: " << script_name << " -f<backup dir> -t<database dir>";
    std::cout << std::endl << "exemple: " << script_name << " -f /var/rocksserver/backup -t /var/rocksserver/db";
    std::cout << std::endl << "Restore RocksDB backup files to database";
    std::cout << std::endl << "";
    std::cout << std::endl << "-f \tbackup dir";
    std::cout << std::endl << "-t \tdatabase dir";
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
    while ( (copt = getopt(argc,argv,"f:t:h")) != -1) {
        ++optcnt;
        switch (copt){
            case 'h':
                HELP_EXIT();
            break;
            case 'f':
                backup_dir = optarg;
            break;
            case 't':
                database_dir = optarg;
            break;
            case '?':
                HELP_EXIT();
            break;
        };
    };
    if(optcnt < 2) {
        HELP_EXIT();
    }

    RocksServer::BackupEngine(backup_dir).restoreBackup(database_dir);

    return 0;
}