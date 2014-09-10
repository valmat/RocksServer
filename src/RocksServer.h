/**
 *  RocksServer.h
 *
 *  Simple RocksDB server
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#define ROCKSSERVER_VERSION "v.0.1.0"

// C++ headers
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

#include <vector>
#include <unordered_map>
#include <forward_list>

// C headers
#include <signal.h>
// C headers for `getrlimit()`
#include <sys/resource.h>

// Libevent
#include <event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

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


// Server wrapper
#include "evhttp/EvBuffer.h"
#include "evhttp/PostData.h"
#include "evhttp/EvRequest.h"
#include "evhttp/EvServerOptions.h"
#include "evhttp/EvLogger.h"

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
#include "listeners/RequestBackup.h"

// Configs
#include "iniparse/IniValue.h"
#include "iniparse/IniConfigs.h"
#include "iniparse/DefaultConfigs.h"


#include "evhttp/EvServer.h"
