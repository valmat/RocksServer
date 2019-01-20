/**
 *  RequestGetIncr.cpp
 *
 *  Request listener for command "/get-incr"
 *  Increments value and returns the result
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/queueServer
 */

#include <rocksserver/api.h>
#include "RequestGetIncr.h"
 
namespace RocksServer {

    /**
     *  Runs request listener
     *  @param       protocol in object
     *  @param       protocol out object
     */
    void RequestGetIncr::run(const ProtocolInPostKeys &in, const ProtocolOut &out) noexcept
    {
        // Detect if current method is POST and any data transfered
        if( !in.check(out) ) {
            return;
        }
        
        auto it = in.begin();
        auto key = *it;
        ++it;

        bool rez = ( in.end() !=  it) ? db.incr( key,  *it ) : db.incr( key );
        
        if( rez ) {
            out.setValue(db.get(key));
        } else {
            out.setFailValue();
            EvLogger::writeLog(db.getStatus().data());
        }
    }
}
