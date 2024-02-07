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
    std::cout << "Usage: " << script_name << " -f<database dir> [-t<output file name>]\n"
              << "Convert RocksDB database to human readable format\n\n"
              << "-f \tdatabase dir\n"
              << "-t \toutput file name prefix\n"
              << "-s \toutput file name sufix (default: .txt)\n"
              << "-b \tbatch size (number of rows)\n"
              << "-h \tprint current help and exit\n";
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

    while ( (copt = getopt(argc,argv,"f:t:b:s::h")) != -1) {
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
            case 's':
                sufix = optarg;
            break;            
            case 'b':
                batch_num = std::stoul(optarg);;
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

    std::cerr << "batch_num " << batch_num << std::endl;

    std::fstream file;
    // file.open(output_prefix, std::ios::out | std::ios::binary);
    // if (!file) {
    //     std::cerr << "Can't open file \"" << output_prefix << '"' << std::endl;
    //     return 3;
    // }

    auto it = db.keyIterator();
    it->SeekToFirst();
    std::string file_name;
    size_t batch_index = 0;
    for (size_t index = 0; it->Valid(); ++index, it->Next()) {
        if(index % batch_num == 0) {
            file_name = gen_name(output_prefix, sufix, batch_index);
            std::cerr << file_name << std::endl;
            file.close();
            file.open(file_name, std::ios::out | std::ios::binary);
            if (!file) {
                std::cerr << "Can't open file \"" << file_name << '"' << std::endl;
                return 3;
            }
            ++batch_index;
        }
        file << '[' << it->key().ToStringView() << "]\n" 
            << it->value().ToStringView().size() << "\n"
            << it->value().ToStringView() << std::endl;
    }



    // auto it = db.keyIterator();
    // for (it->SeekToFirst(); it->Valid(); it->Next()) {
    //     file << '[' << it->key().ToStringView() << "]\n" 
    //         << it->value().ToStringView().size() << "\n"
    //         << it->value().ToStringView() << std::endl;
    // }

    if (!it->status().ok()) {
        std::cerr << it->status().ToString() << std::endl;
        return 4;
    }

    return 0;
}

// 
// bin/human_readable_batches.bin -f"/path/to/db" -b 10000 -t"/path/to/hr_dumps/b_" -s".db"
// 