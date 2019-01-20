/**
 *  RequestMget.cpp
 *
 *  Request listener for command "mget" (multiget)
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
    void RequestMget::run(const ProtocolInGet &in, const ProtocolOut &out)
    {
        // If no key is not transferred
        if(!in.check()) {
            return;
        }
        
        std::vector<rocksdb::Slice> keys;
        for (auto &it : in) {
            keys.emplace_back( it );
        }

        // Retrive result
        std::vector<rocksdb::Status> statuses;
        std::vector<std::string> values = db.mget(keys, statuses);

        // filling output buffer via data, retrieved from the DB
        for(unsigned i=0; i < keys.size(); i++) {
            if(statuses[i].ok()) {
                out.setPair(keys[i], values[i]);
            } else {
                out.setFailPair(keys[i]);
            }
        }
    }

}
