/**
 *  Convert RocksDB database to human readable format
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <iostream>
#include <fstream>
#include <unistd.h>  // getopt
#include "db_wrapper.h"

void print_help(const char *script_name) {
    std::cerr << "Usage: " << script_name << " -f<database dir> [-t<output file name>]\n"
              << "Convert RocksDB database to human readable format\n\n"
              << "-f \tDatabase dir\n"
              << "-t \tOutput file name\n"
              << "-h \tPrint current help and exit\n";
}

int main(int argc, char **argv)
{
    char copt=0;
    std::string database_dir{}, output_fname{};

    while ( (copt = getopt(argc,argv,"f:t::h")) != -1) {
        switch (copt){
            case 'h':
                print_help(*argv);
                return 1;
            case 'f':
                database_dir = optarg;
            break;
            case 't':
                output_fname = optarg;
            break;
            case '?':
                print_help(*argv);
                return 1;
        };
    };

    // Check if input data is valid
    if(database_dir.empty()) {
        print_help(*argv);
        return 1;
    }

    RocksDbContainer db(database_dir);
    // Check if RocksDB is started
    if (!db) {return 2;}        

    std::ostream* out = &std::cout;
    std::ofstream ofs;

    if (!output_fname.empty()) {
        ofs.open(output_fname, std::ios::out | std::ios::binary);
        if (!ofs) {
            std::cerr << "Can't open file \"" << output_fname << '"' << std::endl;
            return 3;
        }
        out = &ofs;
    }

    auto it = db.keyIterator();
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        *out << '[' << it->key().ToStringView() << "]\n" 
            << it->value().ToStringView().size() << "\n"
            << it->value().ToStringView() << std::endl;
    }

    if (!it->status().ok()) {
        std::cerr << it->status().ToString() << std::endl;
        return 4;
    }

    return 0;
}