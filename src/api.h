

#define PLUGIN() extern "C" int myplug()


// Server wrapper
#include "evhttp/EvResponse.h"
#include "evhttp/PostData.h"
#include "evhttp/EvRequest.h"
#include "evhttp/EvServerOptions.h"
#include "evhttp/EvLogger.h"

// Listeners
#include "listeners/RequestBase.h"


// Extensions
#include "extend/Extension.h"