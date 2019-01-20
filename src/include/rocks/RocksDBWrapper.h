/**
 *  RocksDBWrapper.h
 *  Simple RocksDB wrapper
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    // forward declaration
    class Batch;

    class RocksDBWrapper
    {
    public:

        /**
         * Constructor
         * @param IniConfigs
         * @param DefaultConfigs
         */
        RocksDBWrapper(const IniConfigs &cfg, const DefaultConfigs &dfCfg) noexcept;

        ~RocksDBWrapper()
        {
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
        bool set(const rocksdb::Slice &key, const rocksdb::Slice &value)
        {
            _status = _db->Put(rocksdb::WriteOptions(), key, value);
            return _status.ok();
        }

        /**
         * commit batch
         * @param   RocksDB write batch
         */
        bool commit(rocksdb::WriteBatch &batch)
        {
            _status = _db->Write(rocksdb::WriteOptions(), &batch);
            return _status.ok();
        }

        /**
         * commit batch
         * @param   RocksDB write batch
         */
        bool commit(Batch &batch);

        /**
         * Get value by key
         * @param string key
         * @return string value or NULL (if the key is not exist)
         */
        std::string get(const rocksdb::Slice &key) const
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
         * @param statuses
         * @return values
         */
         std::vector<std::string> mget(const std::vector<rocksdb::Slice> &keys, std::vector<rocksdb::Status> &statuses) const;

        /**
         * Get array values by array keys
         * @param  keys
         * @param statuses
         * @return values
         */
         std::vector<std::string> mget(const std::vector<std::string> &keys, std::vector<rocksdb::Status> &statuses) const;

        /**
         * Fast check exist key
         * @param string key
         * @param string value. If the value exists, it can be retrieved. But there is no guarantee that it will be retrieved
         * @param bool value_found.
         * @return bool (true if key exist)
         */
        [[gnu::deprecated("Use keyExist(const rocksdb::Slice &key, std::string &value) instead")]]
        bool keyExist(const rocksdb::Slice &key, std::string &value, bool &value_found) const;

        /**
         * Fast check exist key
         * @param string key
         * @param string value. If the value exists, it can be retrieved. But there is no guarantee that it will be retrieved
         * @return bool (true if key exist)
         */
        bool keyExist(const rocksdb::Slice &key, std::string &value) const;

        /**
         * Fast check exist key
         * @param string key
         * @return bool (true if key exist)
         */
        bool keyExist(const rocksdb::Slice &key) const
        {
            std::string value;
            return keyExist(key, value);
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
        std::string getStatus() const
        {
            return  _status.ToString();
        }

        /**
         * Get last query status state
         */
        bool status() const
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

        /**
         * Get new Iterator
         */
        std::unique_ptr<rocksdb::Iterator> newIter() const
        {
            return std::unique_ptr<rocksdb::Iterator>(_db->NewIterator(rocksdb::ReadOptions()));
        }

    private:
        // DB pointer
        rocksdb::DB* _db;

        // Last operation status
        mutable rocksdb::Status _status;
    };
}
