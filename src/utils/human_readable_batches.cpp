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
#include <cstdlib>  // std::stoul, std::system

#include "db_wrapper.h"

void print_help(const char *script_name) {
    std::cerr << "Usage: " << script_name << " -f<database dir> [-t<output file name>]\n"
              << "Convert RocksDB database to human readable format\n\n"
              << "-f \tDatabase dir\n"
              << "-t \tOutput file name prefix\n"
              << "-e \tOutput file name suffix (default: .txt)\n"
              << "-b \tBatch size (number of rows)\n"
              << "-s \tShift from the beginning (number of rows, default: 0)\n"
              << "-l \tLimit on processing (number of rows, default unlimited)\n"
              << "-c \tCommand to run on file after writing (in format 'cmd {}' )\n"
              << "-h \tPrint current help and exit\n";
}

std::string gen_name(const std::string& prefix, const std::string& suffix, size_t index, int zeros = 10)
{
    std::ostringstream ss;
    ss << prefix;
    ss << std::setw(zeros) << std::setfill('0') << index << suffix;
    return ss.str();
}

bool run_cmd(std::string cmd, const std::string& file_name)
{
    if (cmd.empty() || file_name.empty()) {
        return true;
    }

    size_t pos = cmd.find("{}");
    if (pos != std::string::npos) {
        cmd.replace(pos, 2, file_name);
    } else {
        std::cerr << "Error: Command does not contain '{}' placeholder." << std::endl;
        return false;
    }

    std::cerr << cmd << std::endl;

    return std::system(cmd.c_str()) == 0;
}

int main(int argc, char **argv)
{
    int copt=0;
    std::string database_dir{}, output_prefix{}, cmd{};
    std::string suffix = ".txt";
    size_t batch_num = 50000;
    size_t limit = std::numeric_limits<size_t>::max();
    size_t shift = 0;

    while ( (copt = getopt(argc,argv,"f:t:b:e::l::s::c::h")) != -1) {
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
                suffix = optarg;
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
            case 'c':
                cmd = optarg;
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
    bool has_written_lines = false;
    for (size_t index = 0; it->Valid() && index < limit; ++index, it->Next()) {
        if(index % batch_num == 0) {
            file.flush();
            file.close();
            if(!run_cmd(cmd, file_name)) {
                std::cerr << "Failed to run '" << cmd << "' on file '" << file_name << "'" << std::endl;
                return 3;
            }

            file_name = gen_name(output_prefix, suffix, index + shift);
            std::cerr << file_name << std::endl;
            file.open(file_name, std::ios::out | std::ios::binary);
            if (!file) {
                std::cerr << "Failed to open file: \"" << file_name << '"' << std::endl;
                return 4;
            }
            has_written_lines = false;
        }
        file << '[' << it->key().ToStringView() << "]\n" 
            << it->value().ToStringView().size() << "\n"
            << it->value().ToStringView() << std::endl;

        has_written_lines = true;
        if (file.fail()) {
            std::cerr << "Failed to write to file: \"" << file_name << '"' << std::endl;
            file.flush();
            file.close();
            return 5;
        }            
    }

    if (has_written_lines) {
        file.close();
        if(!run_cmd(cmd, file_name)) {
            std::cerr << "Failed to run '" << cmd << "' on file '" << file_name << "'" << std::endl;
            return 6;
        }
    }    

    if (!it->status().ok()) {
        std::cerr << it->status().ToString() << std::endl;
        return 7;
    }

    return 0;
}
// 
// bin/human_readable_batches.bin -f"/path/to/db" -b10000 -t"/path/to/hr_dumps/b_" -e".db" -s1000 -l2000
// 