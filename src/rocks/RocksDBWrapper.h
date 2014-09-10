/**
 *  RocksDBWrapper.h
 *  Simple RocksDB wrapper
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


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


            dbOptions.create_if_missing = true;
            dbOptions.merge_operator.reset(new Int64Incrementor);
            _status = rocksdb::DB::Open(dbOptions, dbpath, &_db);
        }


        virtual ~RocksDBWrapper()
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
        std::string get(const std::string &key)
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
        bool incr(const rocksdb::Slice& key, const int64_t& incval = 1)
        {
            _status = _db->Merge(rocksdb::WriteOptions(), key, std::to_string(incval));
            return _status.ok();
        }

        /**
         * Incriment value
         * @param   string key
         * @param   string incval
         */
        bool incr(const rocksdb::Slice& key, const rocksdb::Slice& incval)
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
