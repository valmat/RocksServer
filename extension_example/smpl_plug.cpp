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
#include "RequestBackupDel.h"
#include "RequestBackupMdel.h"

using namespace RocksServer;

/*
 * Create plugin
 * 
 * You can use one of the following forms of macro PLUGIN with one, two or three arguments:
 *
 * PLUGIN(Extension extension, RocksDBWrapper& rdb, const IniConfigs& cfg)
 * PLUGIN(Extension extension, RocksDBWrapper& rdb)
 * PLUGIN(Extension extension)
 *
 * The macro `PLUGIN` is defined in rocksserver/api.h. 
 * Instead of using a macro, you can write
 * `extern "C" void plugin(...)` if you like
 *
 * @param extension  object of Extension
 * @param rdb        wrapped object of RocksDB
 * @param cfg        Reference to configuration settings
 */
PLUGIN(Extension extension, RocksDBWrapper& rdb)
{
    extension.bind("/ping",        new RequestPing())
             .bind("/wstats",      new RequestWstats(rdb))
             .bind("/backup/del",  new RequestBackupDel(rdb))
             .bind("/backup/mdel", new RequestBackupMdel(rdb));

}