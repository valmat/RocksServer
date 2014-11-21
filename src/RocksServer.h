/**
 *  RocksServer.h
 *
 *  Simple RocksDB server
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#define ROCKSSERVER_VERSION "v.0.1.4"

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
#include "rocks/Batch.h"
#include "rocks/BackupEngine.h"

// Protocol
#include "protocols/ProtocolOut.h"

#include "protocols/ProtocolInPostSimple.h"
#include "protocols/ProtocolInTrivial.h"
#include "protocols/ProtocolInGetIterator.h"
#include "protocols/ProtocolInGet.h"
#include "protocols/ProtocolInPost.h"
#include "protocols/ProtocolInPostKeysIterator.h"
#include "protocols/ProtocolInPostKeys.h"

#include "protocols/ProtocolInPostPairsIterator.h"
#include "protocols/ProtocolInPostPairs.h"

// Listeners
#include "listeners/RequestBase.h"
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


#include "evhttp/EvServer.h"

// Extensions
#include "extend/Extension.h"
#include "extend/PlugContainer.h"
#include "extend/Extend.h"
//#include "extend/ExtensionObj.h"


