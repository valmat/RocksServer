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
        typedef std::pair<std::string, RequestSuperBase*> ListenerObj;
        typedef typename std::forward_list<ListenerObj>::const_iterator const_iterator;
        
        // Add listener
        void add(std::string path, RequestSuperBase * listener)
        {
            listeners.emplace_front(std::move(path), std::move(listener));
        }

        // Helper methods for create iterator
        const_iterator cbegin() const
        {
            return listeners.cbegin();
        }
        const_iterator cend() const
        {
            return listeners.cend();
        }

    private:
        std::forward_list<ListenerObj> listeners;
    };

}