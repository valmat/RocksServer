/**
 *  RequestIncr.cpp
 *
 *  Request listener for command "/incr"
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
    void RequestIncr::run(const ProtocolInPostKeys &in, const ProtocolOut &out)
    {
        // Detect if current method is POST and any data transfered
        if( !in.check(out) ) {
            return;
        }

        bool rez;
        auto it = in.begin();
        auto key = *it;
        ++it;

        rez = ( in.end() !=  it) ? db.incr( key,  *it ) : db.incr( key );
        
        if( rez ) {
            out.ok();
        } else {
            out.fail();
            EvLogger::writeLog(db.getStatus().data());
        }
    }

}
