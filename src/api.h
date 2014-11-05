

//#define PLUGIN(extension) extern "C" int myplug(RocksServer::Extension *(extension))

// C++ headers
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

#include <vector>
#include <unordered_map>
#include <forward_list>

// C headers
#include <string.h>

// Server wrapper
#include "evhttp/EvResponse.h"
#include "evhttp/PostData.h"
#include "evhttp/EvRequest.h"
#include "evhttp/EvServerOptions.h"
#include "evhttp/EvLogger.h"

// Configs
#include "iniparse/IniValue.h"
#include "iniparse/IniConfigs.h"
#include "iniparse/DefaultConfigs.h"

// RocksDB
#include "rocksdb/db.h"
#include "rocksdb/version.h"
#include "rocksdb/write_batch.h"
#include "rocksdb/merge_operator.h"
#include "rocksdb/utilities/backupable_db.h"

// RocksDB wrapper
#include "rocks/Int64Incrementor.h"
#include "rocks/RocksDBWrapper.h"
#include "rocks/BackupEngine.h"

// Protocol
#include "Protocol/ProtocolOut.h"

#include "Protocol/ProtocolInPostSimple.h"
#include "Protocol/ProtocolInTrivial.h"
#include "Protocol/ProtocolInGetIterator.h"
#include "Protocol/ProtocolInGet.h"
#include "Protocol/ProtocolInPost.h"
#include "Protocol/ProtocolInPostKeysIterator.h"
#include "Protocol/ProtocolInPostKeys.h"

#include "Protocol/ProtocolInPostPairsIterator.h"
#include "Protocol/ProtocolInPostPairs.h"

// Listeners
#include "listeners/RequestBase.h"


// Extensions
#include "extend/Extension.h"


void plugin(const RocksServer::RocksDBWrapper&, const RocksServer::IniConfigs&, RocksServer::Extension *);

extern "C" void get_plugin(const RocksServer::RocksDBWrapper& rdb, const RocksServer::IniConfigs& cfg, RocksServer::Extension *extension)
{
    plugin(rdb, cfg, extension);
}

