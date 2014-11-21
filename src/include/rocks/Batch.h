/**
 *  Batch.h
 *  RocksDB batch wrapper
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    // forward declaration
    //class RocksDBWrapper;

    class Batch
    {
    public:

        // Set a key-value pair to the batch
        Batch & set(const rocksdb::Slice &key, const rocksdb::Slice &value) 
        {
            batch.Put(key, value);
            return *this;
        }

        // Set a key-value pair to the batch
        template <typename KT, typename VT>
        Batch & set(const std::pair<KT,VT> &pair) 
        {
            return set(pair.first, pair.second);
        }

        // Set a key-value pair to the batch
        template <typename KT, typename VT>
        Batch & set(std::pair<KT,VT> &&pair) 
        {
            return set(pair.first, pair.second);
        }

        // Set a key-value pairs to the batch
        template <typename T>
        Batch & set(T &&iterable) 
        {
            for(auto &it : iterable) {
                set(it);
            }
            return *this;
        }

        // Delete a key
        Batch & del(const rocksdb::Slice &key)
        {
            batch.Delete(key);
            return *this;
        }

        // Delete a keys
        template <typename T>
        Batch & del(T &&iterable) 
        {
            for(auto &it : iterable) {
                del(it);
            }
            return *this;
        }

    private:
        // RocksDB batch
        rocksdb::WriteBatch batch;

        friend bool RocksDBWrapper::commit(Batch &); 
    };
}
