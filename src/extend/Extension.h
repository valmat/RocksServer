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

    struct Extension
    {
        typedef std::pair< std::string, std::unique_ptr<RequestSuperBase> > ListenerObj;
        typedef typename std::forward_list<ListenerObj>::iterator iterator;
        
        // Add listener
        void bind(std::string path, RequestSuperBase* listener)
        {
            listeners.emplace_front(std::move(path), std::move(std::unique_ptr<RequestSuperBase>(listener)));

        }

        // Helper methods for create iterator
        iterator begin()
        {
            return listeners.begin();
        }
        
        iterator end()
        {
            return listeners.end();
        }

    private:
        std::forward_list<ListenerObj> listeners;
    };

}