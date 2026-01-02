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
            requires (traits::string_arg_c<KT> && traits::string_arg_c<VT>)
        Batch& set(KT&& key, VT&& value)
        {
            batch.Put(std::forward<KT>(key), std::forward<VT>(value));
            return *this;
        }    

        // Set key-value pairs from iterable
        template <typename T>
            requires traits::range_not_string_c<T>
        Batch& set(T&& iterable)
        {
            for (auto&& it : iterable) {
                set(std::forward<decltype(it)>(it));
            }
            return *this;
        }

        // Set a key-value pair to the batch
        template <typename KT, typename VT>
        Batch& set(const std::pair<KT, VT>& pair)
        {
            return set(pair.first, pair.second);
        }

        template <typename KT, typename VT>
        Batch& set(std::pair<KT, VT>&& pair)
        {
            return set(std::move(pair.first), std::move(pair.second));
        }

        template <typename P>
            requires (traits::pair_like_c<P> && !std::is_same_v<traits::remove_cvref_t<P>,
                std::pair<typename traits::remove_cvref_t<P>::first_type,
                typename traits::remove_cvref_t<P>::second_type>>)
        Batch& set(P&& p)
        {
            return set(std::forward<decltype(p.first)>(p.first),
                       std::forward<decltype(p.second)>(p.second));
        }

        Batch & set(std::string_view key, std::string_view value)
        {
            batch.Put(key, value);
            return *this;
        }

        // Delete a key
        template <typename T>
            requires traits::string_arg_c<T>
        Batch& del(T&& key)
        {
            batch.Delete(std::forward<T>(key));
            return *this;
        }

        // Delete keys (iterable)
        template <typename T>
            requires traits::range_not_string_c<T>
        Batch& del(T&& iterable)
        {
            for (auto&& it : iterable) {
                del(std::forward<decltype(it)>(it));
            }
            return *this;
        }

        Batch& del(std::string_view key)
        {
            batch.Delete(key);
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

        friend bool RocksDBWrapper::commit(Batch&); 
    };
}
