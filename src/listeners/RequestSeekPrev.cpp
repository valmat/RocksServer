/**
 *  RequestSeekPrev.cpp
 *
 *  Request listener for command "seekprev"
 *  https://github.com/facebook/rocksdb/wiki/SeekForPrev
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
    void RequestSeekPrev::run(const ProtocolInGet &in, const ProtocolOut &out) noexcept
    {
        // Check if any key is transferred
        if(!in.check()) {
            return;
        }
        
        auto inIt = in.begin();

        rocksdb::Slice fromPrefix = *(inIt++), startsWith;
        if(in.end() != inIt) {
            startsWith = *inIt;
            std::swap(startsWith, fromPrefix);
        }


        std::cout << "fromPrefix : " << fromPrefix.ToString() << std::endl;
        std::cout << "startsWith : " << startsWith.ToString() << std::endl;
        std::cout << "empty      : " << (startsWith.empty() ? 0 : 1)  << std::endl;

        std::unique_ptr<rocksdb::Iterator> iter(db->NewIterator(rocksdb::ReadOptions()));
        
        iter->SeekForPrev(fromPrefix);
        // If is first entry
        if(!iter->Valid()) {
            iter->SeekToFirst();
        }
        

        std::cout
            << "\t Valid(): "             << iter->Valid()
            << "\t status().ok(): "       << iter->status().ok()
            << std::endl;

        if(startsWith.empty()) {
            for (; iter->Valid(); iter->Next()) {
                if(iter->status().ok()) {
                    out.setPair(iter->key(), iter->value());
                } else {
                    out.setFailPair(iter->key());
                }
            }
        } else {
            for (; iter->Valid() && iter->key().starts_with(startsWith); iter->Next()) {
                if(iter->status().ok()) {
                    out.setPair(iter->key(), iter->value());
                } else {
                    out.setFailPair(iter->key());
                }
            }
        }
    }
}
