
#pragma once

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
#include <rocksserver/evhttp/EvResponse.h>
#include <rocksserver/evhttp/PostData.h>
#include <rocksserver/evhttp/EvRequest.h>
#include <rocksserver/evhttp/EvServerOptions.h>
#include <rocksserver/evhttp/EvLogger.h>

// Configs
#include <rocksserver/iniparse/IniValue.h>
#include <rocksserver/iniparse/IniConfigs.h>
#include <rocksserver/iniparse/DefaultConfigs.h>

// RocksDB
#include <rocksserver/rocksdb/rocksdb/db.h>
#include <rocksserver/rocksdb/rocksdb/version.h>
#include <rocksserver/rocksdb/rocksdb/write_batch.h>
#include <rocksserver/rocksdb/rocksdb/merge_operator.h>
#include <rocksserver/rocksdb/rocksdb/utilities/backupable_db.h>

// RocksDB wrapper
#include <rocksserver/rocks/Int64Incrementor.h>
#include <rocksserver/rocks/RocksDBWrapper.h>
#include <rocksserver/rocks/Batch.h>
#include <rocksserver/rocks/BackupEngine.h>

// Protocol
#include <rocksserver/protocols/ProtocolOut.h>
#include <rocksserver/protocols/ProtocolInPostSimple.h>
#include <rocksserver/protocols/ProtocolInTrivial.h>
#include <rocksserver/protocols/ProtocolInGetIterator.h>
#include <rocksserver/protocols/ProtocolInGet.h>
#include <rocksserver/protocols/ProtocolInPost.h>
#include <rocksserver/protocols/ProtocolInPostKeysIterator.h>
#include <rocksserver/protocols/ProtocolInPostKeys.h>

#include <rocksserver/protocols/ProtocolInPostPairsIterator.h>
#include <rocksserver/protocols/ProtocolInPostPairs.h>

// Listeners
#include <rocksserver/listeners/RequestBase.h>


// Extensions
#include <rocksserver/Extension.h>


#define PLUGIN extern "C" void plugin
