/**
 *  RequestMdel.cpp
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
    void RequestMdel::run(const ProtocolInPostKeys &in, const ProtocolOut &out)
    {
        // Detect if current method is POST
        if(!in.check(out)) {
            return;
        }
        
        // create a RocksDB delete batch
        Batch batch;
        batch.del(in);

        // Apply the delete-batch to the RocksDB
        if(db.commit(batch)) {
            out.ok();
        } else {
            out.fail();
            EvLogger::writeLog(db.getStatus().data());
        }
    }

}
