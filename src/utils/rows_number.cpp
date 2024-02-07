/**
 *  Count RocksDB database keys number
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <iostream>
#include "db_wrapper.h"

int main(int argc, char **argv)
{
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <database dir>" << std::endl;
        return 1;
    }

    RocksDbContainer db(argv[1]);
    // Check if RocksDB is started
    if (!db) {return 2;} 

    size_t rows_num = 0;
    auto it = db.keyIterator();
    for (it->SeekToFirst(); it->Valid(); ++rows_num, it->Next()) {}

    std::cout << rows_num << std::endl;

    if (!it->status().ok()) {
        std::cerr << it->status().ToString() << std::endl;
        return 4;
    }

    return 0;
}