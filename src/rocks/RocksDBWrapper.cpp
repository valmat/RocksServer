/**
 *  RocksDBWrapper.cpp
 *  Simple RocksDB wrapper
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    /**
     * Constructor
     * @param string path
     * @param bool create_if_missing
     */
    RocksDBWrapper::RocksDBWrapper(const IniConfigs &cfg, const DefaultConfigs &dfCfg) noexcept
    {
        // DB path distantion
        std::string dbpath = cfg.get("db_path", dfCfg.db_path);

        // DB options
        rocksdb::Options dbOptions;

        // Set options from ini-config
        dbOptions.max_background_compactions = cfg.get("max_background_compactions", dbOptions.max_background_compactions);
        dbOptions.max_background_flushes     = cfg.get("max_background_flushes", dbOptions.max_background_flushes);
        // dbOptions.allow_os_buffer            = cfg.get("allow_os_buffer", dbOptions.allow_os_buffer);
        dbOptions.max_open_files             = cfg.get("max_open_files", dbOptions.max_open_files);

        dbOptions.write_buffer_size          = cfg.get("write_buffer_size", dbOptions.write_buffer_size);
        dbOptions.max_write_buffer_number    = cfg.get("max_write_buffer_number", dbOptions.max_write_buffer_number);
        dbOptions
          .min_write_buffer_number_to_merge  = cfg.get("min_write_buffer_number_to_merge", dbOptions.min_write_buffer_number_to_merge);

        dbOptions.allow_mmap_reads           = cfg.get("allow_mmap_reads", dbOptions.allow_mmap_reads);
        dbOptions.allow_mmap_writes          = cfg.get("allow_mmap_writes", dbOptions.allow_mmap_writes);
        // dbOptions.block_size                 = cfg.get("block_size", dbOptions.block_size);

        dbOptions.wal_dir                    = cfg.get("wal_dir", dbOptions.wal_dir);
        dbOptions.WAL_ttl_seconds            = cfg.get("wal_ttl_seconds", dbOptions.WAL_ttl_seconds);
        dbOptions.WAL_size_limit_MB          = cfg.get("wal_size_limit_mb", dbOptions.WAL_size_limit_MB);

        dbOptions.create_if_missing          = true;

        dbOptions.merge_operator.reset(new Int64Incrementor);

        _status = rocksdb::DB::Open(dbOptions, dbpath, &_db);
    }

    /**
     * Get array values by array keys
     * @param  keys
     * @param statuses
     * @return values
     */
    std::vector<std::string> RocksDBWrapper::mget(const std::vector<rocksdb::Slice> &keys, std::vector<rocksdb::Status> &statuses) const
    {
        // result values
        std::vector<std::string> values;
        values.reserve(keys.size());
        
        // result statuses
        statuses.reserve(keys.size());

        // filling values
        statuses = _db->MultiGet(rocksdb::ReadOptions(), keys, &values);
        return values;
    }

    /**
     * Get array values by array keys
     * @param  keys
     * @param statuses
     * @return values
     */
    std::vector<std::string> RocksDBWrapper::mget(const std::vector<std::string> &keys, std::vector<rocksdb::Status> &statuses) const
    {
        std::vector<rocksdb::Slice> slice_keys;
        slice_keys.reserve(keys.size());

        // filling slices vector
        for(auto& key: keys) {
            slice_keys.emplace_back(key);
        }

        return mget(slice_keys, statuses);
    }

    /**
     * commit batch
     * @param   RocksDB write batch
     */
    bool RocksDBWrapper::commit(Batch &batch)
    {
        _status = _db->Write(rocksdb::WriteOptions(), &batch.batch);
        return _status.ok();
    }

    /**
     * Fast check exist key
     * @param string key
     * @param string value. If the value exists, it can be retrieved. But there is no guarantee that it will be retrieved
     * @param bool value_found.
     * @return bool (true if key exist)
     */
    bool RocksDBWrapper::keyExist(const rocksdb::Slice &key, std::string &value, bool &value_found) const
    {
        bool isExist = _db->KeyMayExist(rocksdb::ReadOptions(), key, &value, &value_found);
        if(!isExist) {
            return false;
        } else if(isExist && value_found) {
            return true;
        }
        // else

        // If Bloom Filter says that the key is found, it is necessary to further check
        // See: https://github.com/facebook/rocksdb/wiki/RocksDB-Bloom-Filter
        // See: http://rocksdb.org/blog/1427/new-bloom-filter-format/
        _status = _db->Get(rocksdb::ReadOptions(), key, &value);
        return _status.ok();
    }

    /**
     * Fast check exist key
     * @param string key
     * @param string value. If the value exists, it can be retrieved. But there is no guarantee that it will be retrieved
     * @param bool value_found.
     * @return bool (true if key exist)
     */
    bool RocksDBWrapper::keyExist(const rocksdb::Slice &key, std::string &value) const
    {
        bool value_found;
        bool isExist = _db->KeyMayExist(rocksdb::ReadOptions(), key, &value, &value_found);
        if(!isExist) {
            return false;
        } else if(isExist && value_found) {
            return true;
        }
        // else

        // If Bloom Filter says that the key is found, it is necessary to further check
        // See: https://github.com/facebook/rocksdb/wiki/RocksDB-Bloom-Filter
        // See: http://rocksdb.org/blog/1427/new-bloom-filter-format/
        _status = _db->Get(rocksdb::ReadOptions(), key, &value);
        return _status.ok();
    }

}
