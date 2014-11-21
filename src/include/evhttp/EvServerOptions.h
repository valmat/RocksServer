/**
 *  EvServerOptions.h
 *
 *  Options for EvServer
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    struct EvServerOptions
    {
        //Set the value to use for the Content-Type header when none was provided. 
        const char *content_type = nullptr;

        // Limitations for body size (limit in bytes)
        size_t max_body_size = 0;

        // Limitations for headers size
        size_t max_headers_size = 0;

        // Sets the what HTTP methods are supported in requests accepted by this server,
        // and passed to user callbacks.
        uint16_t  allowed_methods = 0;
    };

}
