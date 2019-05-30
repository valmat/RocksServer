/**
 *  RequestSeekNextRange.cpp
 *
 *  Request listener for command "seeknext-range"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    /**
     *  Runs request listener
     *  @param       protocol in object
     *  @param       protocol out object
     */
    void RequestSeekNextRange::run(const ProtocolInGet &in, const ProtocolOut &out) noexcept
    {
        // Check if any key is transferred
        if(!in.check()) {
            return;
        }
        
        auto inIt = in.begin();

        rocksdb::Slice fromPrefix = *(inIt++), toPrefix = *(inIt++), startsWith;
        if(in.end() != inIt) {
            startsWith = *inIt;
        }


        std::cout << "fromPrefix : " << fromPrefix.ToString() << std::endl;
        std::cout << "toPrefix   : " << toPrefix.ToString()   << std::endl;
        std::cout << "startsWith : " << startsWith.ToString() << std::endl;
        std::cout << "empty      : " << (startsWith.empty() ? 0 : 1)  << std::endl;

        std::unique_ptr<rocksdb::Iterator> iter(db->NewIterator(rocksdb::ReadOptions()));
        
        iter->SeekForPrev(fromPrefix);
        if(!iter->Valid()) {
            iter->SeekToFirst();
        } else {
            iter->Next();
        }
        

        std::cout
            << "\t Valid(): "             << iter->Valid()
            << "\t status().ok(): "       << iter->status().ok()
            << std::endl;

        if(startsWith.empty()) {
            for (; iter->Valid() && iter->key().compare(toPrefix) <= 0; iter->Next()) {
            // for (; iter->Valid(); iter->Next()) {

                // std::cout
                //     << "iter->key()   : " << iter->key().ToString()   
                //     << "toPrefix      : " << toPrefix.ToString()   
                //     << "iter->key().compare(toPrefix)   : " << iter->key().compare(toPrefix)   
                //     << std::endl;


                if(iter->status().ok()) {
                    out.setPair(iter->key(), iter->value());
                } else {
                    out.setFailPair(iter->key());
                }
            }
        } else {
            for (; iter->Valid() && iter->key().starts_with(startsWith) && iter->key().compare(toPrefix) <= 0; iter->Next()) {
                if(iter->status().ok()) {
                    out.setPair(iter->key(), iter->value());
                } else {
                    out.setFailPair(iter->key());
                }
            }
        }




    }
}
