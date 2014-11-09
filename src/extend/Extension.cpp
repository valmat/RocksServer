/**
 *
 *  Extension.cpp
 *  Base class for RocksServer extension
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

namespace RocksServer {

    void Extension::bind(std::string &&path, listener_ptr &&listener)
    {
        server.bind(path.data(), std::move(listener) );
    }

}