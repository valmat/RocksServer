/**
 *  ProtocolInGetIterator.h
 *
 *  This is an internal helper class that is used when iterating over a keys
 *
 *  Thus, when you do c++ things like "for (auto &iter : prot_in)", internally
 *  a ProtocolInGetIterator object is being used.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

namespace RocksServer {

    class ProtocolInGetIterator
    {
    public:

        typedef std::string::size_type size_type;

        ProtocolInGetIterator(const std::string &uri, size_type startpos) :
            str(uri),
            lpos(startpos),
            rpos(str.find('&', lpos))
        {
            setCurrent();
        }

        /**
         *  Trivial constructor (for returning in method ProtocolInGet::end())
         */
        ProtocolInGetIterator() :
            str(std::string()),
            lpos(npos)
        {}

        /**
         *  Increment position (pre-increment)
         */
        ProtocolInGetIterator &operator++()
        {
            if(rpos == npos) {
                lpos = npos;
                return *this;
            }

            lpos = rpos+1;
            rpos = str.find('&', lpos);

            setCurrent();
            return *this;
        }
        
        /**
         *  Increment position (post-increment)
         */
        ProtocolInGetIterator operator++(int)
        {
            // make a copy
            ProtocolInGetIterator copy(*this);
            
            // increment current object
            ++(*this);
            
            // return the original
            return copy;
        }
        
        /**
         *  Compare with other iterator
         */
        bool operator!=(const ProtocolInGetIterator &that) const
        {
            return (lpos != that.lpos);
        }

        /**
         *  Derefecence, this returns a rocksdb::Slice with the current key
         */
        const rocksdb::Slice & operator*() const
        {
            return current;
        }
        
        /**
         *  Derefecence, this returns a rocksdb::Slice with the current key
         */
        const rocksdb::Slice * operator->() const
        {
            return &current;
        }

    private:

        /**
         *  Retrive current
         */
        void setCurrent()
        {
            current = (rpos == npos) ?
                rocksdb::Slice(str.data()+lpos, str.size()-lpos) :
                rocksdb::Slice(str.data()+lpos, rpos-lpos);
        }

        const size_type &npos = std::string::npos;
        
        const std::string &str;

        // startpos
        size_type lpos;
        size_type rpos;

        rocksdb::Slice current;

    };

}

