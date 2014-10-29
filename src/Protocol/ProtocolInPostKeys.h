 /**
 *  ProtocolInPostKeys.h
 *
 *  Bridge to work with  a request.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class ProtocolInPostKeys: public ProtocolInPost
    {
    public:
        ProtocolInPostKeys(const EvRequest &r) : ProtocolInPost(r) {}

        ProtocolInPostKeysIterator begin() const
        {
            return ProtocolInPostKeysIterator(raw);
        }

        ProtocolInPostKeysIterator end() const
        {
            return ProtocolInPostKeysIterator();
        }

        ~ProtocolInPostKeys() {}
    };

}
