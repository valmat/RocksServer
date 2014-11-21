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
        /**
         *  SFINAE
         *  This type trait checks if the  `begin` and `end` methods is defined in class T
         */
        template <typename T>
        class is_possible_iterable
        {
            typedef char yes;
            class no { char _[2]; };

            typedef typename std::remove_reference<typename std::remove_const<T>::type>::type X;

            template <typename R> static yes test( decltype(&R::begin), decltype(&R::end) ) ;
            template <typename R> static no test(...);

        public:
            enum { value = sizeof(test<X>(0,0)) == sizeof(yes) };
        };

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
        typename std::enable_if<is_possible_iterable<T>::value, Batch &>::type
        set(T &&iterable) 
        {
            for(auto &it : iterable) {
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
        typename std::enable_if<!is_possible_iterable<T>::value, Batch &>::type
        del(T &&key)
        {
            batch.Delete(std::forward<T>(key));
            return *this;
        }

        // Delete a keys
        template <typename T>
        typename std::enable_if<is_possible_iterable<T>::value, Batch &>::type
        del(T &&iterable) 
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