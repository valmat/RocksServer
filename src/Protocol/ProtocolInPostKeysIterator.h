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

    /**
     *  Forward declarations
     */
    //class ProtocolInPostKeysIteratorImpl;

    class ProtocolInPostKeysIterator
    {
    public:

        typedef std::string::size_type size_type;

        /**
         *  Constructor
         *  @param  impl        Implementation iterator
         */
        ProtocolInPostKeysIterator(const PostData &raw) :
            raw(raw),
            lpos(0),
            rpos(raw.find('\n'))

        {
            std::cout << "[[[\t\t\t\t\t\t\x1b[1;34;47m ProtocolInPostKeysIterator(const PostData &raw) : " << npos << "\t" << lpos << "\x1b[0m" << std::endl;
            setCurrent();
        }

        // trivial constructor
        ProtocolInPostKeysIterator() :
            raw(PostData(nullptr, 0)),
            lpos(npos),
            rpos(npos)
        {
            std::cout << "[[[\t\t\t\t\t\t\x1b[1;34;47m ProtocolInPostKeysIterator() : " << npos << "\t" << lpos << "\x1b[0m" << std::endl;
        }

        ~ProtocolInPostKeysIterator() {}

        /**
         *  Increment position (pre-increment)
         */
        ProtocolInPostKeysIterator &operator++()
        {
            ++cntr;
            std::cout << "{{{{\t\t\tlpos:"<<lpos<<"\trpos:" << rpos << std::endl;
            if(rpos == npos) {
                lpos = npos;
                return *this;
            }

            lpos = rpos+1;
            rpos = raw.find('\n', lpos);
            
            std::cout << "{{{{\t\t\t\tlpos:"<<lpos<<"\trpos:" << rpos << std::endl;

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
            std::cout << "[[[\t\t\t\t\t\t\x1b[1;31;47m"<< lpos << "!= that."<< that.lpos << "\x1b[0m" << std::endl;
            std::cout << "[[[\t\t\t\t\t\t\x1b[31m"<< this << "\t&that:"<< &that << "\x1b[0m" << std::endl;
            std::cout << "[[[\t\t\t\t\t\t\x1b[1;34;47m npos: "<< npos << "\tstd::string::npos:"<< std::string::npos << "\x1b[0m" << std::endl;

            return (cntr < 6) && (lpos != that.lpos);
        }

        /**
         *  Derefecence, this returns a rocksdb::Slice with the current key
         */
        const rocksdb::Slice & operator*() const
        {
            //std::cout << "{{{{\t\t\t\t\t\t"<<(const char *)raw<<"\t:"<<current.ToString() << std::endl;
            std::cout << "{{{{\t\t\t\t\t\t"<< current.ToString() << std::endl;
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

        //const size_type strlen;

        // startpos
        size_type lpos;
        size_type rpos;

        rocksdb::Slice current;

        int cntr = 0;
    };

}

