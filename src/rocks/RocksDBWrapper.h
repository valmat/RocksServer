/**
 *  RocksDBWrapper.h
 *  Simple RocksDB wrapper
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class RocksDBWrapper
    {
    public:

        /**
         * Constructor
         * @param string path
         * @param bool create_if_missing
         */
        RocksDBWrapper(const IniConfigs &cfg, const DefaultConfigs &dfCfg)
        {
            // DB path distantion
            std::string dbpath = cfg.get("db_path", dfCfg.db_path);

            // DB options
            rocksdb::Options dbOptions;

            // Set options from ini-config
            dbOptions.max_background_compactions = cfg.get("max_background_compactions", dbOptions.max_background_compactions);
            dbOptions.max_background_flushes     = cfg.get("max_background_flushes", dbOptions.max_background_flushes);
            dbOptions.allow_os_buffer            = cfg.get("allow_os_buffer", dbOptions.allow_os_buffer);
            dbOptions.max_open_files             = cfg.get("max_open_files", dbOptions.max_open_files);

            dbOptions.write_buffer_size          = cfg.get("write_buffer_size", dbOptions.write_buffer_size);
            dbOptions.max_write_buffer_number    = cfg.get("max_write_buffer_number", dbOptions.max_write_buffer_number);
            dbOptions
              .min_write_buffer_number_to_merge  = cfg.get("min_write_buffer_number_to_merge", dbOptions.min_write_buffer_number_to_merge);

            dbOptions.allow_mmap_reads           = cfg.get("allow_mmap_reads", dbOptions.allow_mmap_reads);
            dbOptions.allow_mmap_writes          = cfg.get("allow_mmap_writes", dbOptions.allow_mmap_writes);
            dbOptions.block_size                 = cfg.get("block_size", dbOptions.block_size);

            dbOptions.wal_dir                    = cfg.get("wal_dir", dbOptions.wal_dir);
            dbOptions.WAL_ttl_seconds            = cfg.get("wal_ttl_seconds", dbOptions.WAL_ttl_seconds);
            dbOptions.WAL_size_limit_MB          = cfg.get("wal_size_limit_mb", dbOptions.WAL_size_limit_MB);


            dbOptions.create_if_missing          = true;

            dbOptions.merge_operator.reset(new Int64Incrementor);

            _status = rocksdb::DB::Open(dbOptions, dbpath, &_db);

            // If is data base backupable
            if(cfg.get("isbackupable", dfCfg.isbackupable)) {
                auto bcOptions = rocksdb::BackupableDBOptions(cfg.get("backup_path", dfCfg.backup_path));

                // If share_table_files == true, backup will assume that table files with
                // same name have the same contents. This enables incremental backups and
                // avoids unnecessary data copies.
                // If share_table_files == false, each backup will be on its own and will
                // not share any data with other backups.
                // default: true
                bcOptions.share_table_files = cfg.get("share_table_files",  bcOptions.share_table_files);
                // If sync == true, we can guarantee you'll get consistent backup even
                // on a machine crash/reboot. Backup process is slower with sync enabled.
                // If sync == false, we don't guarantee anything on machine reboot. However,
                // chances are some of the backups are consistent.
                // Default: true
                bcOptions.sync              = cfg.get("backup_sync",        bcOptions.sync);
                // If true, it will delete whatever backups there are already
                // Default: false
                bcOptions.destroy_old_data  = cfg.get("backup_destroy_old", bcOptions.destroy_old_data);
                // If false, we won't backup log files. This option can be useful for backing
                // up in-memory databases where log file are persisted, but table files are in memory.
                // (See https://github.com/facebook/rocksdb/wiki/How-to-persist-in-memory-RocksDB-database%3F)
                // Default: true
                bcOptions.backup_log_files  = cfg.get("backup_log_files",   bcOptions.backup_log_files);

                _db = new rocksdb::BackupableDB(_db, bcOptions);
            }


        }

        ~RocksDBWrapper()
        {
            //delete Incrementor; <-- not required : std::shared_ptr
            delete _db;
        }

        /**
         *  Cast to a rocksdb::DB pointer
         */
        operator rocksdb::DB * () const
        {
            return _db;
        }

        rocksdb::DB* operator->()
        { 
            return _db; 
        }

        /**
         * Get value by key
         * @param string key
         * @param string value
         */
         bool set(const rocksdb::Slice &key, const rocksdb::Slice &value) {
            _status = _db->Put(rocksdb::WriteOptions(), key, value);
            return _status.ok();
        }

        /**
         * Set multiple values
         * @param   RocksDB write batch
         */
        bool mset(rocksdb::WriteBatch &batch)
        {
            _status = _db->Write(rocksdb::WriteOptions(), &batch);
            return (bool)(_status.ok());
        }

        /**
         * Get value by key
         * @param string key
         * @return string value or NULL (if the key is not exist)
         */
        std::string get(const rocksdb::Slice &key)
        {
            std::string value;

            _status = _db->Get(rocksdb::ReadOptions(), key, &value);
            if (!_status.ok()) {
                return "";
            }
            return value;
        }

        /**
         * Get array values by array keys
         * @param  keys
         * @return statuses
         */
         std::vector<std::string> mget(const std::vector<rocksdb::Slice> &keys, std::vector<rocksdb::Status> &statuses)
        {
            // size of keys array
            size_t arrSize = keys.size();
            
            // result values
            std::vector<std::string> values;
            values.reserve(arrSize);
            
            // result statuses
            statuses.reserve(arrSize);

            // filling values
            statuses = _db->MultiGet(rocksdb::ReadOptions(), keys, &values);
            return values;
        }

        /**
         * Fast check exist key
         * @param string key
         * @param string value. If the value exists, it can be retrieved. But there is no guarantee that it will be retrieved
         * @return bool (true if key exist)
         */
        bool keyExist(const rocksdb::Slice &key, std::string &value, bool &value_found)
        {
            return _db->KeyMayExist(rocksdb::ReadOptions(), key, &value, &value_found);
        }

        /**
         * Remove key from db
         * @param string key
         */
        bool del(const rocksdb::Slice& key)
        {
            _status = _db->Delete(rocksdb::WriteOptions(), key);
            return _status.ok();
        }

        /**
         * Get last query status string
         */
        std::string getStatus()
        {
            return  _status.ToString();
        }

        /**
         * Get last query status state
         */
        bool status()
        {
            return _status.ok();
        }

        /**
         * Incriment value
         * @param   string key
         * @param   incval, default: 1
         */
        bool incr(const rocksdb::Slice& key, const int64_t& incval)
        {
            _status = _db->Merge(rocksdb::WriteOptions(), key, std::to_string(incval));
            return _status.ok();
        }

        /**
         * Incriment value
         * @param   string key
         * @param   string incval
         */
        bool incr(const rocksdb::Slice& key, const rocksdb::Slice& incval = "1")
        {
            _status = _db->Merge(rocksdb::WriteOptions(), key, incval);
            return _status.ok();
        }

    private:
        // DB pointer
        rocksdb::DB* _db;

        // Last operation status
        rocksdb::Status _status;
    };
}
