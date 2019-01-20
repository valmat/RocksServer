/**
 *  smpl_plug.cpp
 *
 *  Simple example of RocksServer plugin
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <rocksserver/api.h>
#include "RequestPing.h"
#include "RequestWstats.h"
#include "RequestSeekFirst.h"
#include "RequestGetIncr.h"

using namespace RocksServer;

/*
 * Create plugin
 * 
 * You can use one of the following forms of macro PLUGIN with one, two or three arguments:
 *
 * PLUGIN(Extension extension, RocksDBWrapper& db, const IniConfigs& cfg)
 * PLUGIN(Extension extension, RocksDBWrapper& db)
 * PLUGIN(Extension extension)
 *
 * The macro `PLUGIN` is defined in rocksserver/api.h. 
 * Instead of using the macro, you can write
 * `extern "C" void plugin(...)` if you like
 *
 * @param extension  object of Extension
 * @param db         wrapped object of RocksDB
 * @param cfg        Reference to configuration settings
 */
PLUGIN(Extension extension, RocksDBWrapper& db)
{
    extension.bind("/ping",       new RequestPing())
             .bind("/wstats",     new RequestWstats(db))
             .bind("/seek-first", new RequestSeekFirst(db))
             .bind("/get-incr",   new RequestGetIncr(db));
}