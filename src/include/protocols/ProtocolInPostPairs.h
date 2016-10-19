 /**
 *  ProtocolInPostPairs.h
 *
 *  Bridge to work with  a request.
 *  For http method POST
 *
 *  If the request http method is the POST 
 *  and you need to get a key-value pairs sequence from the POST request, use this class
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once
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

        std::map<std::string, std::string> map() const
        {
            std::map<std::string, std::string> values;
            for(auto &&it: *this) {
                values.emplace(it.first.ToString(), it.second.ToString());
            }
            return values;
        }
    };

}
