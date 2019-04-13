/**
 *  RequestSet.cpp
 *
 *  Request listener for command "set"
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
    void RequestSet::run(const ProtocolInPost &in, const ProtocolOut &out) noexcept
    {
        // Detect if current method is correct POST
        if( !in.check(out) ) {
            return;
        }
        
        // Get key-value pair from the POST data
        auto pair = in.pair();

        // Set key-value pair to the DB
        if(db.set(pair.first, pair.second)) {
            out.ok();
        } else {
            out.fail();
            EvLogger::writeLog(db.getStatus().data());
        }
    }

}
