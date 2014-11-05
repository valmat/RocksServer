 /**
 *  ProtocolInPostPairs.h
 *
 *  Bridge to work with  a request.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class ProtocolInPostPairs: public ProtocolInPost
    {
    public:
        ProtocolInPostPairs(const EvRequest &r) : ProtocolInPost(r) {}

        ProtocolInPostPairsIterator begin() const
        {
            return ProtocolInPostPairsIterator(raw);
        }

        ProtocolInPostPairsIterator end() const
        {
            return ProtocolInPostPairsIterator();
        }
    };

}
