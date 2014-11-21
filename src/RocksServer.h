/**
 *  RocksServer.h
 *
 *  Simple RocksDB server
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "include/version.h"

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
#include <signal.h>
// C headers for `getrlimit()`
#include <sys/resource.h>

// Libevent
#include <event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

// Server wrapper
#include "include/evhttp/EvResponse.h"
#include "include/evhttp/PostData.h"
#include "include/evhttp/EvRequest.h"
#include "include/evhttp/EvServerOptions.h"
#include "include/evhttp/EvLogger.h"

// Configs
#include "include/iniparse/IniValue.h"
#include "include/iniparse/IniConfigs.h"
#include "include/iniparse/DefaultConfigs.h"

// RocksDB
#include "rocksdb/db.h"
#include "rocksdb/version.h"
#include "rocksdb/write_batch.h"
#include "rocksdb/merge_operator.h"
#include "rocksdb/utilities/backupable_db.h"

// RocksDB wrapper
#include "include/rocks/Int64Incrementor.h"
#include "include/rocks/RocksDBWrapper.h"
#include "include/rocks/Batch.h"
#include "include/rocks/BackupEngine.h"

// Protocol
#include "include/protocols/ProtocolOut.h"

#include "include/protocols/ProtocolInPostSimple.h"
#include "include/protocols/ProtocolInTrivial.h"
#include "include/protocols/ProtocolInGetIterator.h"
#include "include/protocols/ProtocolInGet.h"
#include "include/protocols/ProtocolInPost.h"
#include "include/protocols/ProtocolInPostKeysIterator.h"
#include "include/protocols/ProtocolInPostKeys.h"

#include "include/protocols/ProtocolInPostPairsIterator.h"
#include "include/protocols/ProtocolInPostPairs.h"

// Listeners
#include "include/listeners/RequestBase.h"
#include "listeners/RequestGet.h"
#include "listeners/RequestMget.h"
#include "listeners/RequestSet.h"
#include "listeners/RequestMset.h"
#include "listeners/RequestKeyExist.h"
#include "listeners/RequestDel.h"
#include "listeners/RequestMdel.h"
#include "listeners/RequestIncr.h"
#include "listeners/RequestTailing.h"
#include "listeners/RequestStats.h"
#include "listeners/RequestPrefIt.h"
#include "listeners/RequestBackup.h"
#include "listeners/RequestBackupInfo.h"


#include "include/evhttp/EvServer.h"

// Extensions
#include "include/Extension.h"
#include "extend/PlugContainer.h"
#include "extend/Extend.h"

