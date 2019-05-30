/**
 *  RequestSeekNext.cpp
 *
 *  Request listener for command "seeknext"
 *  https://github.com/facebook/rocksdb/wiki/SeekForPrev
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
    void RequestSeekNext::run(const ProtocolInGet &in, const ProtocolOut &out) noexcept
    {
        // Check if any key is transferred
        if(!in.check()) {
            return;
        }
        
        auto fromPrefix = in.key();
        std::cout << fromPrefix.ToString() << std::endl;
        std::unique_ptr<rocksdb::Iterator> iter(db->NewIterator(rocksdb::ReadOptions()));


        
        iter->SeekForPrev(fromPrefix);
        if(!iter->Valid()) {
            iter->SeekToFirst();
        } else {
            iter->Next();
        }
        

        std::cout
            << "\t Valid(): "             << iter->Valid()
            << "\t status().ok(): "       << iter->status().ok()
            << std::endl;

        for (; iter->Valid(); iter->Next()) {
            std::cout << "-------------------" << std::endl;
            if(iter->status().ok()) {
                std::cout << "\tkey: " << iter->key().ToString() << "\tvalue: " << iter->value().ToString() << std::endl;
                out.setPair(iter->key(), iter->value());
            } else {
                out.setFailPair(iter->key());
                std::cout << "setFailPair" << std::endl;
            }
        }
    }
}
