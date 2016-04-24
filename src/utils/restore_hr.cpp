/**
 *
 *  Restore RocksDB database from a human readable backup
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <iostream>
#include <fstream>
#include <unistd.h>  // getopt

// RocksDB
#include "rocksdb/db.h"
#include "rocksdb/write_batch.h"

#define HELP_EXIT()  print_help(*argv); return 0;

void print_help(const char *script_name)
{
    std::cout << "usege: " << script_name << " -f<human_readable backup file> -t<database_dir> [-n<comitt_lenght>] [-s<max_batch_size>]";
    std::cout << std::endl << "exemple: " << script_name << " -f\"human_readable_backup.txt\" -t/var/rocksserver/db";
    std::cout << std::endl << "Restore RocksDB backup from a human_readable backup file";
    std::cout << std::endl;
    std::cout << std::endl << "-f \thuman_readable backup file";
    std::cout << std::endl << "-t \tdatabase dir";
    std::cout << std::endl << "-n \tcomitt lenght. Write the batch after every <comitt lenght> entries.";
    std::cout << std::endl << "   \tTo prevent out of memory.";
    std::cout << std::endl << "   \tDefault:0 to write all entries.";
    std::cout << std::endl << "-s \tmax batch size. The maximum batch size in bytes after which the batch will be written.";
    std::cout << std::endl << "   \tTo prevent out of memory.";
    std::cout << std::endl << "   \tDefault:0 to write all entries.";
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
    std::string backup_file, database_dir;
    size_t clenght = 0; // comitt lenght
    size_t bsize = 0;   // max batch size
    opterr=0;
    while ( (copt = getopt(argc,argv,"f:t:n::s::h")) != -1) {
        ++optcnt;
        switch (copt){
            case 'h':
                HELP_EXIT();
            break;
            case 'f':
                if(optarg) {
                    backup_file = optarg;
                }
            break;
            case 't':
                if(optarg) {
                    database_dir = optarg;
                }
            break;
            case 'n':
                if(optarg) {
                    clenght = std::strtoull(optarg, nullptr, 10);
                }
            break;
            case 's':
                if(optarg) {
                    bsize = std::strtoull(optarg, nullptr, 10);
                }
            break;
            case '?':
                HELP_EXIT();
            break;
        };
    };
    if(optcnt < 2) {
        HELP_EXIT();
    }

    if(backup_file.empty() || database_dir.empty()) {
        HELP_EXIT();
    }


    std::ifstream file(backup_file);
    if(!file) {
        std::cerr<<"Can't open file "<< backup_file << std::endl;
        return 1;
    }

    // DB pointer
    rocksdb::DB* pdb;

    // DB options
    rocksdb::Options dbOptions;
    dbOptions.create_if_missing = true;

    // Last operation status
    rocksdb::Status status = rocksdb::DB::Open(dbOptions, database_dir, &pdb);
    std::unique_ptr<rocksdb::DB> db(pdb);

    if (!status.ok()) {
        std::cerr << "RocksDB start error:" << std::endl << status.ToString() << std::endl;
        return 1;
    }

    // RocksDB batch
    rocksdb::WriteBatch batch;

    std::string key;
    std::string value;
    std::string value_part;
    std::string len_str;
    unsigned val_len;
    for(size_t i = 1; std::getline(file, key); ++i ) {
        
        // Retrive value length
        std::getline(file, len_str);
        val_len = std::strtol(len_str.c_str(), nullptr, 10);

        // Retrive value
        std::getline(file, value);
        for(unsigned len = value.size(); len < val_len &&  std::getline(file, value_part); len += value_part.size()+1) {
            value.push_back('\n');
            value.append(value_part);
        }

        // Set key-value to batch
        batch.Put(key.substr( 1, key.size()-2 ), value);

        // Partial write the batch to prevent out of memory
        if( (clenght > 0 && 0 == i%clenght) || (bsize > 0 && batch.GetDataSize() >= bsize) ) {
            status = db->Write(rocksdb::WriteOptions(), &batch);
            // renew the batch
            batch = rocksdb::WriteBatch();

            if (!status.ok()) {
                std::cerr << "RocksDB commit error:" << std::endl << status.ToString() << std::endl;
                return 1;
            }
        }
    }

    status = db->Write(rocksdb::WriteOptions(), &batch);
    if (!status.ok()) {
        std::cerr << "RocksDB commit error:" << std::endl << status.ToString() << std::endl;
        return 1;
    }

    return 0;
}