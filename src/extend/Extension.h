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
        //typedef typename std::forward_list<ListenerObj>::const_iterator const_iterator;
        typedef typename std::forward_list<ListenerObj>::iterator iterator;
        
        // Add listener
        void add(std::string path, std::unique_ptr<RequestSuperBase> listener)
        {
            listeners.emplace_front(std::move(path), std::move(listener));
        }

        // Helper methods for create iterator
        iterator begin() //const
        {
            return listeners.begin();
        }
        iterator end() //const
        {
            return listeners.end();
        }

        ~Extension()
        {
            std::cerr <<  "~Extension" << std::endl;
        }

    private:
        std::forward_list<ListenerObj> listeners;
    };

}