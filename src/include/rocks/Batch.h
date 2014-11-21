/**
 *  Batch.h
 *  RocksDB batch wrapper
 *  
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once






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






namespace RocksServer {

    // forward declaration
    //class RocksDBWrapper;

    class Batch
    {
    public:

        // Set a key-value pair to the batch
        /*
        */
        Batch & set(const rocksdb::Slice &key, const rocksdb::Slice &value) 
        {
            batch.Put(key, value);
            return *this;
        }
        /*
        template <typename KT, typename VT>
        Batch & set(KT &&key, VT &&value) 
        {
            batch.Put(std::forward<KT>(key), std::forward<VT>(value));
            return *this;
        }
        */


        // Set a key-value pairs to the batch
        template <typename T>
        typename std::enable_if<is_possible_iterable<T>::value, Batch &>::type 
        set(T &&iterable) 
        //Batch & set(T &&iterable) 
        {
            for(auto &it : iterable) {
                set(it);
            }
            return *this;
        }
        
        Batch & set(const std::pair<rocksdb::Slice,rocksdb::Slice> &pair)
        {
            return set(pair.first, pair.second);
        }
        Batch & set(const std::pair<std::string,std::string> &pair)
        {
            return set(pair.first, pair.second);
        }
        


        /*
        template <typename KT, typename VT>
        Batch & set< std::pair<KT,VT> >(const std::pair<KT,VT> &pair) 
        {
            return set(pair.first, pair.second);
        }
        */

        /*
        template <typename KT, typename VT>
        Batch & set< std::pair<KT,VT> >(const std::pair<KT,VT> &pair) 
        {
            return set(pair.first, pair.second);
        }
        typename std::enable_if<std::is_same<T, float>::value, void>::type
        */



        // Set a key-value pair to the batch
        /*
        template <typename KT, typename VT>
        Batch & set(std::pair<KT,VT> &&pair) 
        {
            return set(pair.first, pair.second);
        }
        */

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

        /*
        template<>
        inline Batch & Batch::set< std::pair<rocksdb::Slice,rocksdb::Slice> >(const std::pair<rocksdb::Slice,rocksdb::Slice> &pair) 
        //inline Batch & Batch::set(const std::pair<rocksdb::Slice,rocksdb::Slice> &pair) 
        {
            return set(pair.first, pair.second);
        }
        template<>
        inline Batch & Batch::set< std::pair<std::string,std::string> >(const std::pair<std::string,std::string> &pair) 
        //inline Batch & Batch::set(const std::pair<std::string,std::string> &pair) 
        {
            return set(pair.first, pair.second);
        }
        */
}
