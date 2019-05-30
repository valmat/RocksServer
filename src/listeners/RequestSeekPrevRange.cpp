/**
 *  RequestSeekPrevRange.cpp
 *
 *  Request listener for command "seekprev-range"
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
    void RequestSeekPrevRange::run(const ProtocolInGet &in, const ProtocolOut &out) noexcept
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


        std::unique_ptr<rocksdb::Iterator> iter(db->NewIterator(rocksdb::ReadOptions()));
        
        iter->SeekForPrev(fromPrefix);
        // If is first entry
        if(!iter->Valid()) {
            iter->SeekToFirst();
        }
        

        if(startsWith.empty()) {
            for (; iter->Valid() && iter->key().compare(toPrefix) <= 0; iter->Next()) {
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
