/**
 *  RequestDel.cpp
 *
 *  Request listener for command "/del"
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
    void RequestDel::run(const ProtocolInPost &in, const ProtocolOut &out) noexcept
    {
        // Detect if current method is POST
        if( !in.check(out) ) {
            return;
        }
        
        if( db.del(in.key()) ) {
            out.ok();
        } else {
            out.fail();
            EvLogger::writeLog(db.getStatus().data());
        }
    }

}
