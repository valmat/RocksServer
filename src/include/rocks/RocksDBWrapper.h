/**
 *  RocksDBWrapper.h
 *  Simple RocksDB wrapper
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

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
        RocksDBWrapper(const IniConfigs& cfg, const DefaultConfigs& dfCfg);
        ~RocksDBWrapper() = default;

        RocksDBWrapper(const RocksDBWrapper&) = delete;
        RocksDBWrapper& operator=(const RocksDBWrapper&) = delete;

        RocksDBWrapper(RocksDBWrapper&&) noexcept = default;
        RocksDBWrapper& operator=(RocksDBWrapper&&) noexcept = default;

        /**
         *  Cast to a rocksdb::DB pointer
         */
        operator rocksdb::DB* () const { return _db.get(); }

        rocksdb::DB* operator->() { return _db.get(); }
        const rocksdb::DB* operator->() const { return _db.get(); }

        rocksdb::DB* db() noexcept { return _db.get(); }
        const rocksdb::DB* db() const noexcept { return _db.get(); }

        bool isOpen() const noexcept { return _db != nullptr && _status.ok(); }
        const rocksdb::Status& lastStatus() const noexcept { return _status; }

        // Legacy API

        /**
         * Set value by key
         * @param string key
         * @param string value
         */
        bool set(const rocksdb::Slice& key, const rocksdb::Slice& value)
        {
            _status = _db->Put(rocksdb::WriteOptions(), key, value);
            return _status.ok();
        }

        /**
         * Get value by key
         * @param string key
         * @return string value or NULL (if the key is not exist)
         */
        std::string get(const rocksdb::Slice& key) const
        {
            std::string value;
             _status = _db->Get(rocksdb::ReadOptions(), key, &value);
            if (!_status.ok()) {
                return "";
            }
            return value;
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
         * commit batch
         * @param   RocksDB write batch
         */
        bool commit(rocksdb::WriteBatch& batch)
        {
            _status = _db->Write(rocksdb::WriteOptions(), &batch);
            return _status.ok();
        }

        /**
         * commit batch
         * @param   RocksDB write batch
         */
        bool commit(Batch& batch);

        /**
         * Get array values by array keys
         * @param  keys
         * @param statuses
         * @return values
         */
        std::vector<std::string> mget(const std::vector<rocksdb::Slice>& keys, std::vector<rocksdb::Status>& statuses) const;

        /**
         * Get array values by array keys
         * @param  keys
         * @param statuses
         * @return values
         */
        std::vector<std::string> mget(const std::vector<std::string>& keys, std::vector<rocksdb::Status>& statuses) const;

        /**
         * Fast check exist key
         * @param string key
         * @param string value. If the value exists, it can be retrieved. But there is no guarantee that it will be retrieved
         * @param bool value_found.
         * @return bool (true if key exist)
         */
        [[deprecated("Use keyExist(const rocksdb::Slice &key, std::string &value) instead")]]
        bool keyExist(const rocksdb::Slice& key, std::string& value, bool& value_found) const;

        /**
         * Fast check exist key
         * @param string key
         * @param string value. If the value exists, it can be retrieved. But there is no guarantee that it will be retrieved
         * @return bool (true if key exist)
         */
        bool keyExist(const rocksdb::Slice& key, std::string& value) const;

        /**
         * Fast check exist key
         * @param string key
         * @return bool (true if key exist)
         */
        bool keyExist(const rocksdb::Slice& key) const
        {
            std::string value;
            return keyExist(key, value);
        }

        /**
         * Incriment value
         * @param   string key
         * @param   incval, default: 1
         */
        bool incr(const rocksdb::Slice& key, int64_t incval)
        {
            std::string val_s = std::to_string(incval);
            _status = _db->Merge(rocksdb::WriteOptions(), key, val_s);
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

        // 
        // NEW API
        // 
        std::vector<std::optional<std::string>>
        mget(const std::vector<rocksdb::Slice>& keys,
             std::vector<rocksdb::Status>* statuses_out = nullptr) const;

        std::vector<std::optional<std::string>>
        mget(std::span<const std::string_view> keys,
             std::vector<rocksdb::Status>* statuses_out = nullptr) const;

        std::vector<std::optional<std::string>>
        mget(const std::vector<std::string>& keys,
             std::vector<rocksdb::Status>* statuses_out = nullptr) const;

        bool set(std::string_view key, std::string_view value);

        std::optional<std::string> get(std::string_view key) const;

        bool del(std::string_view key);

        template <class... Keys>
        requires (std::convertible_to<Keys, std::string_view> && ...)
        auto mget(Keys&&... keys) const
            -> std::array<std::optional<std::string>, sizeof...(Keys)>
        {
            constexpr std::size_t N = sizeof...(Keys);

            std::vector<rocksdb::Slice> slices{
                rocksdb::Slice{std::forward<Keys>(keys)}...
            };

            std::vector<std::string> values;
            values.resize(N);

            auto statuses = _db->MultiGet(rocksdb::ReadOptions(), slices, &values);

            std::array<std::optional<std::string>, N> result{};

            for (std::size_t i = 0; i < N; ++i) {
                if (statuses[i].ok()) [[likely]] {
                    result[i] = std::move(values[i]);
                } else {
                    result[i] = std::nullopt;
                }
            }

            return result;
        }
        
    private:
        std::unique_ptr<rocksdb::DB> _db;
        mutable rocksdb::Status _status;
    };

} // namespace RocksServer
