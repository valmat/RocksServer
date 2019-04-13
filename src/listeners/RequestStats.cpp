/**
 *  RequestStats.cpp
 *
 *  Request listener for command "stats" (Get statistics)
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
    void RequestStats::run(const ProtocolInTrivial &in, const ProtocolOut &out) noexcept
    {
        std::string stat;
        if(db->GetProperty("rocksdb.stats", &stat)) {
            out.setStr(stat);
        } else {
            out.fail();
            EvLogger::writeLog(db.getStatus().data());
        }
    }

}
