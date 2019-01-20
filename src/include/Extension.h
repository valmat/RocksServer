/**
 *
 *  Extension.h
 *  Base class for RocksServer extension
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {

    // Forward declaration
    class EvServer;

    class Extension
    {
        typedef std::unique_ptr<RequestSuperBase> listener_ptr;
    public:

        Extension(EvServer &server) : server(server) {};

        // Add listener
        Extension& bind(std::string path, RequestSuperBase* listener)
        {
            // create unique_ptr in plugin scope
            bind(std::move(path), listener_ptr(listener));
            return *this;
        }

    private:
        // Add listener
        void bind(std::string &&path, listener_ptr &&listener);

        EvServer &server;
    };

}