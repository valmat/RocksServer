/**
 *  RequestGet.cpp
 *
 *  Request listener for command "get"
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
    void RequestGet::run(const ProtocolInGet &in, const ProtocolOut &out) noexcept
    {
        // Check if any data transfered
        if(!in.check()) {
            out.setFailValue();
            return;
        }

        std::string val = db.get(in.key());

        if(!db.status()) {
            out.setFailValue();
        } else {
            out.setValue(val);
        }
    }

}
