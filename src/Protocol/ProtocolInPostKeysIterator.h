/**
 *  ProtocolInPostKeysIterator.h
 *
 *  This is an internal helper class that is used when iterating over a keys
 *
 *  Thus, when you do c++ things like "for (auto &iter : prot_in)", internally
 *  a ProtocolInPostKeysIterator object is being used.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

namespace RocksServer {

    class ProtocolInPostKeysIterator
    {
    public:

        typedef std::string::size_type size_type;

        ProtocolInPostKeysIterator(const PostData &raw) :
            raw(raw),
            lpos(0),
            rpos(raw.find('\n'))

        {
            setCurrent();
        }

        /**
         *  Trivial constructor (for returning in method ProtocolInPostKeys::end())
         */
        ProtocolInPostKeysIterator() :
            raw(PostData()),
            lpos(npos)
        {}

        ~ProtocolInPostKeysIterator() {}

        /**
         *  Increment position (pre-increment)
         */
        ProtocolInPostKeysIterator &operator++()
        {
            if(rpos == npos) {
                lpos = npos;
                return *this;
            }

            lpos = rpos+1;
            rpos = raw.find('\n', lpos);
            
            setCurrent();
            return *this;
        }
        
        /**
         *  Increment position (post-increment)
         */
        ProtocolInPostKeysIterator operator++(int)
        {
            // make a copy
            ProtocolInPostKeysIterator copy(*this);
            
            // increment current object
            ++(*this);
            
            // return the original
            return copy;
        }
        
        /**
         *  Compare with other iterator
         */
        bool operator!=(const ProtocolInPostKeysIterator &that) const
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
                rocksdb::Slice(raw+lpos, raw.size()-lpos) :
                rocksdb::Slice(raw+lpos, rpos-lpos);
        }

        const size_type &npos = std::string::npos;

        const PostData &raw;
        
        size_type lpos;
        size_type rpos;

        rocksdb::Slice current;
    };

}

