/**
 *  RequestPrefIt.cpp
 *
 *  Request listener for command "prefit"
 *  Prefix iterator
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
    void RequestPrefIt::run(const ProtocolInGet &in, const ProtocolOut &out) noexcept
    {
        // Check if any key is transferred
        if(!in.check()) {
            return;
        }
        
        auto prefix = in.key();
        auto iter(db.newIter());
        
        // Iterate over prefixed keys
        for (iter->Seek(prefix); iter->Valid() && iter->key().starts_with(prefix); iter->Next()) {
            if(iter->status().ok()) {
                out.setPair(iter->key(), iter->value());
            } else {
                out.setFailPair(iter->key());
            }
        }
    }

}
