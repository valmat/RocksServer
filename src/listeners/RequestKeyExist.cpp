/**
 *  RequestKeyExist.cpp
 *
 *  Request listener for command "exit" (check if key exist)
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
    void RequestKeyExist::run(const ProtocolInGet &in, const ProtocolOut &out)
    {
        if(!in.check()) {
            out.fail();
            return;
        }
            
        std::string value;
        if(db.keyExist(in.key(), value)) {
            out.ok();
            out.setValue(value);
        } else {
            out.fail();
        }
    }

}
