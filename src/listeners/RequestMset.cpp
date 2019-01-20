/**
 *  RequestMset.cpp
 *
 *  Request listener for command "mset" (multiset)
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
    void RequestMset::run(const ProtocolInPostPairs &in, const ProtocolOut &out)
    {
        // Detect if current method is POST
        if( !in.check(out)) {
            return;
        }
        
        // create a RocksDB write batch
        Batch batch;
        batch.set(in);

        // Set batch to the DB and filling output buffer
        if(db.commit(batch)) {
            out.ok();
        } else {
            out.fail(); 
            EvLogger::writeLog(db.getStatus().data());
        }
    }

}
