/**
 *  Batch.h
 *  RocksDB batch wrapper
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    class Batch
    {
    public:
        // Set a key-value pair to the batch
        template <typename KT, typename VT>
        Batch & set(KT &&key, VT &&value) 
        {
            batch.Put(std::forward<KT>(key), std::forward<VT>(value));
            return *this;
        }

        // Set a key-value pairs to the batch
        template <typename T>
        typename std::enable_if<traits::may_iterable<T>::value, Batch &>::type
        set(T &&iterable)
        {
            for(auto &&it : iterable) {
                set(it);
            }
            return *this;
        }
        
        template <typename KT, typename VT>
        Batch & set(const std::pair<KT,VT> &pair)
        {
            return set(pair.first, pair.second);
        }

        // Set a key-value pair to the batch
        template <typename KT, typename VT>
        Batch & set(std::pair<KT,VT> &&pair)
        {
            return set(std::move(pair.first), std::move(pair.second));
        }

        // Delete a key
        template <typename T>
        typename std::enable_if<!traits::may_iterable<T>::value, Batch &>::type
        del(T &&key)
        {
            batch.Delete(std::forward<T>(key));
            return *this;
        }

        // Delete a keys
        template <typename T>
        typename std::enable_if<traits::may_iterable<T>::value, Batch &>::type
        del(T &&iterable)
        {
            for(auto &&it : iterable) {
                del(it);
            }
            return *this;
        }

        // Retrieve the serialized version of this batch.
        const std::string& data() const
        {
            return batch.Data();
        }

        // Returns the number of updates in the batch
        int count() const
        {
            return batch.Count();
        }

    private:
        // RocksDB batch
        rocksdb::WriteBatch batch;

        friend bool RocksDBWrapper::commit(Batch &); 
    };

}