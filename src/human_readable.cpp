/**
 *
 *  Convert RocksDB database to human readable format
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <iostream>
#include <fstream>
#include <unistd.h>  // getopt

// RocksDB
#include "rocksdb/db.h"
#include "rocksdb/version.h"
#include "rocksdb/write_batch.h"
#include "rocksdb/merge_operator.h"

// RocksDB Incrementor
#include "rocks/Int64Incrementor.h"


#define HELP_EXIT()  print_help(*argv); return 0;

void print_help(const char *script_name)
{
    std::cout << "usege: " << script_name << " -f<database dir> [-t<output file name>]";
    std::cout << std::endl << "Convert RocksDB database to human readable format";
    std::cout << std::endl << "";
    std::cout << std::endl << "-f \tdatabase dir";
    std::cout << std::endl << "-t \toutput file name";
    std::cout << std::endl << "-h \tprint current help and exit";
    std::cout << std::endl;
}


// Simplest RocksDB wrapper container
class RocksDbContainer
{
public:
    RocksDbContainer(const std::string &database_dir)
    {
        // DB options
        rocksdb::Options dbOptions;
        dbOptions.create_if_missing = true;
        dbOptions.merge_operator.reset(new RocksServer::Int64Incrementor);

        auto status = rocksdb::DB::Open(dbOptions, database_dir, &_db );
        
        _valid = status.ok();
        if (!_valid) {
            std::cerr << "RocksDB start error:" << std::endl << status.ToString() << std::endl;
            std::cerr << "RocksDB version is " << ROCKSDB_MAJOR << "." << ROCKSDB_MINOR << "." << ROCKSDB_PATCH << std::endl;
        }
    }

    operator bool()
    {
        return _valid;
    }

    std::unique_ptr<rocksdb::Iterator> keyIterator()
    {
        return std::unique_ptr<rocksdb::Iterator>(_db->NewIterator(rocksdb::ReadOptions()));;
    }

    ~RocksDbContainer()
    {
        delete _db;
    }
private:
    rocksdb::DB* _db;
    bool _valid = false;
};


int main(int argc, char **argv)
{
    /*
     * Get options
     */
    char copt=0;
    unsigned optcnt = 0;
    std::string database_dir, output_fname = "";

    opterr=0;
    while ( (copt = getopt(argc,argv,"f:t::h")) != -1) {
        ++optcnt;
        switch (copt){
            case 'h':
                HELP_EXIT();
            break;
            case 'f':
                database_dir = optarg;
            break;
            case 't':
                output_fname = optarg;
            break;
            case '?':
                HELP_EXIT();
            break;
        };
    };
    if(optcnt < 1) {
        HELP_EXIT();
    }

    /*
     * Init RocksDB
     */
    RocksDbContainer db(database_dir);

    // Check if RocksDB started
    if (!db) {
        return 1;
    }

    // Redirected stdout to file
    FILE *fp = nullptr;
    if(output_fname.size()) {
        if ( !(fp = freopen(output_fname.c_str(), "w", stdout)) ) {
            std::cerr<<"Can't open file "<< output_fname << std::endl;
            return 1;
        }
    }

    /*
     * Iterate over all db keys
     */
    std::string key, value;
    auto it = db.keyIterator();

    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        key   = it->key().ToString();
        value = it->value().ToString();
        //std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
        std::cout << '[' << key << ']' << std::endl;
        std::cout << value.size() << std::endl;
        std::cout << value << std::endl;
    }

    // Check for any errors found during the scan
    if(!it->status().ok()) {
        std::cerr<< it->status().ToString() << std::endl;
    }
    
    if(fp) fclose(fp);

    return 0;
}