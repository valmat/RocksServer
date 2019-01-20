/**
 *  RequestWstats.h
 *
 *  Request listener for command "wstats" (Get wide statistics)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <rocksserver/api.h>
#include "RequestWstats.h"

namespace RocksServer {

    /**
     *  Runs request listener
     *  @param       protocol in object
     *  @param       protocol out object
     */
    void RequestWstats::run(const ProtocolInGet &in, const ProtocolOut &out)
    {
        // Valid property names include:
        //
        //  "rocksdb.num-files-at-level<N>" - return the number of files at level <N>,
        //     where <N> is an ASCII representation of a level number (e.g. "0").
        //  "rocksdb.stats" - returns a multi-line string that describes statistics
        //     about the internal operation of the DB.
        //  "rocksdb.sstables" - returns a multi-line string that describes all
        //     of the sstables that make up the db contents.
        //
        //  Possible requests:
        //  http://127.0.0.1:5577/wstats?stats
        //  http://127.0.0.1:5577/wstats?sstables
        //  http://127.0.0.1:5577/wstats?num-files-at-level0
        //  http://127.0.0.1:5577/wstats?num-files-at-level1
        //  etc...


        std::string key;
        if(in.check()) {
            key = in.key().ToString();
            key = "rocksdb."+key;
        } else {
            key = "rocksdb.sstables";
        }

        std::string stat;
        if(_rdb->GetProperty(key, &stat)) {
            out.setStr(stat);
        } else {
            out.fail();
            EvLogger::writeLog(_rdb.getStatus().data());
        }
    }

}
