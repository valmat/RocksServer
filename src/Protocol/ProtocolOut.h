 /**
 *  ProtocolOut.h
 *
 *  Bridge to work with the message buffer.
 *  All messages go through the ProtocolOut, but not on a straight line in the buffer.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class ProtocolOut
    {
    public:
        ProtocolOut(const EvResponse &r) :resp(r) {}
        ProtocolOut(EvResponse &&r) :resp(std::move(r)) {}

        void setValue(const std::string &val) const
        {
            resp.add_printf("%lu\n%s", val.size(), val.c_str());
        }
        
        void setFailValue() const
        {
            resp.add("-1\n", 3);
        }

        template<typename KeyType, typename ValueType>
        void setPair(const KeyType &key, const ValueType &val) const
        {
            resp.add(key.data(), key.size())
                .add_printf("\n%lu\n", val.size())
                .add(val.data(), val.size())
                .endl();
        }
        
        template<typename KeyType>
        void setFailPair(const KeyType &key) const
        {
            resp.add(key.data(), key.size())
                .add("\n-1\n", 4);
        }

        template<typename ValueType>
        void setStr(ValueType &&val) const
        {
            resp.add(std::forward<ValueType>(val));
        }

        void ok() const
        {
            resp.add("OK\n", 3);
        }
        void fail() const
        {
            resp.add("FAIL\n", 5);
        }

    private:
        const EvResponse &resp;
    };

}
