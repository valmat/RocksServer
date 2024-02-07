/**
 *  Convert RocksDB database to human readable format
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <string>
#include <memory>

// RocksDB
#include "rocksdb/db.h"
#include "rocksdb/merge_operator.h"

// RocksDB Incrementor
#include "include/rocks/Int64Incrementor.h"

// RAII wrapper for RocksDB
class RocksDbContainer
{
public:
    RocksDbContainer(const std::string &database_dir)
    {
        // DB options
        rocksdb::Options dbOptions;
        dbOptions.merge_operator.reset(new RocksServer::Int64Incrementor);

        rocksdb::DB* raw_db;
        auto status = rocksdb::DB::OpenForReadOnly(dbOptions, database_dir, &raw_db);
        
        if (status.ok()) {
            _db.reset(raw_db);
            _valid = true;
        } else {
            std::cerr << "RocksDB start error:" << std::endl << status.ToString() << std::endl;
            std::cerr << "RocksDB version is " << ROCKSDB_MAJOR << "." << ROCKSDB_MINOR << "." << ROCKSDB_PATCH << std::endl;
        }
    }

    operator bool() {
        return _valid;
    }

    std::unique_ptr<rocksdb::Iterator> keyIterator() {
        return std::unique_ptr<rocksdb::Iterator>(_db->NewIterator(rocksdb::ReadOptions()));
    }

private:
    std::unique_ptr<rocksdb::DB> _db;
    bool _valid = false;
};