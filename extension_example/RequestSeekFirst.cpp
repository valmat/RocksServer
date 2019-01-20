/**
 *  RequestSeekFirst.cpp
 *
 *  Request listener for command "/seek-first"
 *  Returns the first pair corresponding to the key prefix
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/queueServer
 */

#include <rocksserver/api.h>
#include "RequestSeekFirst.h"

namespace RocksServer {

    /**
     *  Runs request listener
     *  @param       protocol in object
     *  @param       protocol out object
     */
    void RequestSeekFirst::run(const ProtocolInGet &in, const ProtocolOut &out) noexcept
    {
        // If no key is transferred
        if(!in.check()) {
            out.setFailPair();
            return;
        }

        auto prefix = *in.begin();
        auto iter(db.newIter());
        iter->Seek(prefix);
        
        iter->Valid() && iter->key().starts_with(prefix) && iter->status().ok() ? 
            out.setPair(iter->key(), iter->value()) :
            out.setFailPair();
    }
}
