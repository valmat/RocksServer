/**
 *  ProtocolInPostPairsIterator.h
 *
 *  This is an internal helper class that is used when iterating over a keys
 *
 *  Thus, when you do c++ things like "for (auto &iter : prot_in)", internally
 *  a ProtocolInPostPairsIterator object is being used.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

namespace RocksServer {

    class ProtocolInPostPairsIterator
    {
    public:

        typedef std::string::size_type size_type;

        ProtocolInPostPairsIterator(const PostData &raw) :
            raw(raw),
            rawlen(raw.size())

        {
            if(lpos < rawlen) {
                next();
            }
        }

        /**
         *  Trivial constructor (for returning in method ProtocolInPostPairs::end())
         */
        ProtocolInPostPairsIterator(size_type rawlen) :
            raw(PostData(nullptr, 0)),
            lpos(rawlen)
        {}

        ~ProtocolInPostPairsIterator() {}

        /**
         *  Increment position (pre-increment)
         */
        ProtocolInPostPairsIterator &operator++()
        {
            cntr++;

            std::cout << "\t\x1b[1;30m NEXT \x1b[0m" << std::endl;

            /*
            if(rpos == npos) {
                lpos = npos;
                return *this;
            }
            */

            //std::cout << "[[[\t\t\t\t\t\t\x1b[1;31;47m lpos:"<< lpos << "\trpos:"<< that.lpos << "\x1b[0m" << std::endl;

            // retrive current pair
            next();







            //std::cout << "[[[\t\t\t\t\t\t\x1b[1;34;47m lpos:"<< lpos << "\trpos:"<< that.lpos << "\x1b[0m" << std::endl;
            
            
            return *this;
        }
        
        /**
         *  Increment position (post-increment)
         */
        ProtocolInPostPairsIterator operator++(int)
        {
            // make a copy
            ProtocolInPostPairsIterator copy(*this);
            
            // increment current object
            ++(*this);
            
            // return the original
            return copy;
        }
        
        /**
         *  Compare with other iterator
         */
        bool operator!=(const ProtocolInPostPairsIterator &that) const
        {
            std::cout << "\t\x1b[1;30m CHECK \x1b[0m" << std::endl;

            std::cout << "[[[\t\t\t\t\t\t\x1b[1;34m"<< lpos << "!= that."<< that.lpos << "\x1b[0m" << std::endl;
            //return (cntr < 10) && (lpos != that.lpos);
            return (cntr < 10) && (lpos < that.lpos);
        }

        /**
         *  Derefecence, this returns a rocksdb::Slice with the current key
         */
        const std::pair<rocksdb::Slice, rocksdb::Slice> & operator*() const
        {
            std::cout << "\t\x1b[1;30m RETRIVE \x1b[0m" << std::endl;
            //std::cout << "{{{{\t\t\t\t\t\t"<<(const char *)raw<<"\t:"<<current.ToString() << std::endl;
            std::cout << "{{{{\t\t\t\t\t\t\x1b[1;31;47m"<< current.first.ToString() << "\t"<< current.second.ToString()  << "\x1b[0m" << std::endl;

            return current;
        }
        
        /**
         *  Derefecence, this returns a rocksdb::Slice with the current key
         */
        const std::pair<rocksdb::Slice, rocksdb::Slice> * operator->() const
        {
            return &current;
        }

    private:

        /**
         *  Retrive current
         */
        void next()
        {
            std::cout << "\t\x1b[1;32m "<< lpos <<" \x1b[0m" << std::endl;
            // retrive key
            rpos = raw.find('\n', lpos);
            if(npos == rpos) {
                return;
            }
            key_star = lpos;
            key_len  = rpos - lpos;

            
            // retrive value
            lpos = rpos+1;
            std::cout << "\t\x1b[1;32m "<< lpos <<" \x1b[0m" << std::endl;

            rpos = raw.find('\n', lpos);
            vallen = std::atol(raw.data() + lpos);
            lpos = rpos+1;
            std::cout << "\t\x1b[1;32m "<< lpos <<" \x1b[0m" << std::endl;

            // set pair
            current = std::make_pair(rocksdb::Slice(raw.data() + key_star, key_len), rocksdb::Slice(raw.data() + lpos, vallen));

            //to next iteration
            lpos += vallen + 1;
            std::cout << "\t\x1b[1;32m "<< lpos << "\t" << rawlen <<" \x1b[0m" << std::endl;
        }

        int cntr = 0;

        const size_type &npos = std::string::npos;

        const PostData &raw;
        
        size_type rawlen = raw.size();
        size_type lpos = 0;
        size_type rpos;

        size_type key_star;
        size_type key_len;
        size_type vallen = 0;

        std::pair<rocksdb::Slice, rocksdb::Slice> current;
    };

}


/*

            

            while(lpos < rawlen) {
                
                // retrive key
                size_type key_star, key_len;
                rpos = raw.find('\n', lpos);
                key_star = lpos;
                key_len  = rpos - lpos;

                
                // retrive value
                lpos = rpos+1;
                rpos = raw.find('\n', lpos);
                long vallen = std::atol((const char *)raw + lpos);
                lpos = rpos+1;
                
                // filling batch
                batch.Put(rocksdb::Slice((const char *)raw + key_star, key_len), rocksdb::Slice(raw + lpos, vallen));

                //to next iteration
                lpos += vallen + 1;
            }
*/


