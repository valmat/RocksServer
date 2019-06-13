/**
 *  RequestSeekNext.cpp
 *
 *  Request listener for command "seeknext"
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
    void RequestSeekNext::run(const ProtocolInGet &in, const ProtocolOut &out) noexcept
    {
        // Check if any key is transferred
        if(!in.check()) {
            return;
        }
        
        auto inIt = in.begin();

        rocksdb::Slice fromPrefix = *(inIt++), startsWith;
        if(in.end() != inIt) {
            startsWith = *inIt;
        }

        auto iter(db.newIter());
        
        iter->SeekForPrev(fromPrefix);
        if(!iter->Valid()) {
            iter->SeekToFirst();
        } else {
            if(iter->key() != fromPrefix) {
                iter->Next();
            }
        }
        
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
