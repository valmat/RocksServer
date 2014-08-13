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
    private:
        /**
         * db pointer
         */
        rocksdb::DB* _db;

        /**
         * db path distantion
         */
        std::string _dbpath;

        /**
         * db options
         */
        rocksdb::Options _dboptions;

        /**
         * last operation status
         */
        rocksdb::Status _status;

    public:

        /**
         * function __construct
         * @param string path
         * @param bool create_if_missing
         */
        RocksDBWrapper(std::string dbpath, bool create_if_missing = true):
        _dbpath(dbpath)
        {
            _dboptions.create_if_missing = create_if_missing;
            _dboptions.merge_operator.reset(new Int64Incrementor);
            _status = rocksdb::DB::Open(_dboptions, _dbpath, &_db);

            //std::cout << _status.ToString() << std::endl;

            if (!_status.ok())  {
                std::cerr << _status.ToString() << std::endl;
            }
        }

        virtual ~RocksDBWrapper()
        {
            //delete Incrementor; <-- not required : std::shared_ptr
            delete _db;
        }

        /**
         * set value by key
         * @param string key
         * @param string value
         */
         bool set(const rocksdb::Slice &key, const rocksdb::Slice &value) {
            //std::cout << "set("<< key << ")=" << value << std::endl;

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
         * get value by key
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
         * get array values by array keys
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
         * fast check exist key
         * @param string key
         * @param string value. If the value exists, it can be retrieved. But there is no guarantee that it will be retrieved
         * @return bool (true if key exist)
         */
        bool keyExist(const rocksdb::Slice &key, std::string &value, bool &value_found)
        {
            return _db->KeyMayExist(rocksdb::ReadOptions(), key, &value, &value_found);
        }


        /**
         * remove key from db
         * @param string key
         */
        bool del(const rocksdb::Slice& key)
        {
            _status = _db->Delete(rocksdb::WriteOptions(), key);
            return _status.ok();
        }

        /**
         * get last query status string
         */
        std::string getStatus()
        {
            return  _status.ToString();
        }

        /**
         * get last query status state
         */
        bool status()
        {
            return _status.ok();
        }
        


        /**
         * incriment value
         * @param   string key
         * @param   incval, default: 1
         */
        bool incr(const rocksdb::Slice& key, const int64_t& incval = 1)
        {
            _status = _db->Merge(rocksdb::WriteOptions(), key, std::to_string(incval));
            return _status.ok();
        }

        /**
         * incriment value
         * @param   string key
         * @param   string incval
         */
        bool incr(const rocksdb::Slice& key, const rocksdb::Slice& incval)
        {
            _status = _db->Merge(rocksdb::WriteOptions(), key, incval);
            return _status.ok();
        }

    };
}
