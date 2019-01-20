/**
 *  RequestMdelset.cpp
 *
 *  Request listener for command "mdelset" (multi del and set)
 *  Allows to remove and set a keys for a single query
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
    void RequestMdelset::run(const ProtocolInPostPairs &in, const ProtocolOut &out)
    {
        // Detect if current method is POST
        if( !in.check(out)) {
            return;
        }

        // create a RocksDB write batch
        Batch batch;

        auto it = in.begin();
        auto itend = in.end();
        // Remove keys
        for(; it != itend && it->first.empty(); ++it) {
            batch.del(it->second);
        }
        // Set key-value pairs
        for(; it != itend; ++it) {
            batch.set(*it);
        }

        // Set batch to the DB and filling output buffer
        if(db.commit(batch)) {
            out.ok();
        } else {
            out.fail();
            EvLogger::writeLog(db.getStatus().data());
        }
    }

}