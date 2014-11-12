 /**
 *  ProtocolInPostKeys.h
 *
 *  Bridge to work with  a request.
 *  For http method POST
 *
 *  If the request http method is the POST 
 *  and you need to get a keys sequence from the POST request, use this class
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
    };

}
