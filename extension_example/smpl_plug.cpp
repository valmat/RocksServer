/**
 *  smpl_plug.cpp
 *
 *  Simple example RocksServer plugin
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include <rocksserver/api.h>
#include "RequestPing.h"


using namespace RocksServer;

/*
 * Create plugin
 */
//PLUGIN(Extension &extension, const RocksDBWrapper& rdb, const IniConfigs& cfg)
//PLUGIN(Extension &extension, const RocksDBWrapper& rdb)
PLUGIN(Extension &extension)
{
    extension.bind("/ping", new RequestPing());
}