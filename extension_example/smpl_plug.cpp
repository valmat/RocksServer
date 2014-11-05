// g++ -std=c++11 -shared -fPIC -o smpl_plug.so smpl_plug.cpp

// g++ -std=c++11 -fPIC -I../deps/rocksdb -c smpl_plug.cpp
// g++ -std=c++11 -shared smpl_plug.o -o smpl_plug.so

 // g++ -std=c++11 -fPIC -I../deps/rocksdb/include -c smpl_plug.cpp

// nm smpl_plug.so

#include "../src/api.h"
#include "RequestPing.h"


using namespace RocksServer;

//PLUGIN(Extension &extension, const RocksDBWrapper& rdb, const IniConfigs& cfg)
PLUGIN(Extension &extension)
{
    extension.bind("/ping", new RequestPing());
}