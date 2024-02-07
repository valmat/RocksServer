/**
 *  Convert RocksDB database to human readable format
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <unistd.h>  // getopt
#include <cstdlib>  // std::stoul

#include "db_wrapper.h"

void print_help(const char *script_name) {
    std::cerr << "Usage: " << script_name << " -f<database dir> [-t<output file name>]\n"
              << "Convert RocksDB database to human readable format\n\n"
              << "-f \tDatabase dir\n"
              << "-t \tOutput file name prefix\n"
              << "-e \tOutput file name sufix (default: .txt)\n"
              << "-b \tBatch size (number of rows)\n"
              << "-s \tShift from the beginning (number of rows, default: 0)\n"
              << "-l \tLimit on processing (number of rows, default unlimited)\n"
              << "-h \tPrint current help and exit\n";
}

std::string gen_name(const std::string& prefix, const std::string& sufix, size_t index, int zeros = 10)
{
    std::ostringstream ss;
    ss << prefix;
    ss << std::setw(zeros) << std::setfill('0') << index << sufix;
    return ss.str();
}


int main(int argc, char **argv)
{
    char copt=0;
    std::string database_dir{}, output_prefix{};
    std::string sufix = ".txt";
    size_t batch_num = 50000;
    size_t limit = std::numeric_limits<size_t>::max();
    size_t shift = 0;

    while ( (copt = getopt(argc,argv,"f:t:b:e::l::s::h")) != -1) {
        switch (copt){
            case 'h':
                print_help(*argv);
                return 1;
            case 'f':
                database_dir = optarg;
            break;
            case 't':
                output_prefix = optarg;
            break;
            case 'e':
                sufix = optarg;
            break;
            case 'b':
                batch_num = std::stoul(optarg);
            break;
            case 'l':
                limit = std::stoul(optarg);
            break;
            case 's':
                shift = std::stoul(optarg);
            break;
            case '?':
                print_help(*argv);
                return 1;
        };
    };

    // Check if input data are valid
    if(database_dir.empty() || output_prefix.empty()) {
        print_help(*argv);
        return 1;
    }

    RocksDbContainer db(database_dir);
    // Check if RocksDB is started
    if (!db) {return 2;} 

    auto it = db.keyIterator();
    it->SeekToFirst();
    
    if(shift > 0) {
        for (size_t index = 0; it->Valid() && index < shift; ++index, it->Next()) {}
    }
    
    std::fstream file;
    std::string file_name;
    for (size_t index = 0; it->Valid() && index < limit; ++index, it->Next()) {
        if(index % batch_num == 0) {
            file_name = gen_name(output_prefix, sufix, index + shift);
            std::cerr << file_name << std::endl;
            file.close();
            file.open(file_name, std::ios::out | std::ios::binary);
            if (!file) {
                std::cerr << "Can't open file \"" << file_name << '"' << std::endl;
                return 3;
            }
        }
        file << '[' << it->key().ToStringView() << "]\n" 
            << it->value().ToStringView().size() << "\n"
            << it->value().ToStringView() << std::endl;
    }

    if (!it->status().ok()) {
        std::cerr << it->status().ToString() << std::endl;
        return 4;
    }

    return 0;
}
// 
// bin/human_readable_batches.bin -f"/path/to/db" -b10000 -t"/path/to/hr_dumps/b_" -e".db" -s1000 -l2000
// 