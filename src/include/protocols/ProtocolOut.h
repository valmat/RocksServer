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

        template<typename T, typename = typename std::enable_if<traits::may_string<T>::value, void>::type>
        const ProtocolOut& setValue(T &&val) const
        {
            resp.add_printf("%lu\n%.*s\n", val.size(), val.size(), val.data());
            return *this;
        }
        const ProtocolOut& setValue(const char *val) const
        {
            size_t len = strlen(val);
            resp.add_printf("%lu\n%.*s\n", len, len, val);
            return *this;
        }
        
        const ProtocolOut& setFailValue() const
        {
            resp.add("-1\n", 3);
            return *this;
        }

        template<typename KeyType, typename ValueType>
        const ProtocolOut& setPair(const KeyType &key, const ValueType &val) const
        {
            resp.add(key.data(), key.size())
                .add_printf("\n%lu\n", val.size())
                .add(val.data(), val.size())
                .endl();
            return *this;
        }
        
        template<typename KeyType>
        const ProtocolOut& setFailPair(const KeyType &key) const
        {
            resp.add(key.data(), key.size())
                .add("\n-1\n", 4);
            return *this;
        }
        const ProtocolOut& setFailPair() const
        {
            resp.add("\n-1\n", 4);
            return *this;
        }

        template<typename ...ValueType>
        const ProtocolOut& setStr(ValueType&&... val) const
        {
            resp.add(std::forward<ValueType>(val)...);
            return *this;
        }

        template<typename ValueType>
        const ProtocolOut& setKey(ValueType &&val) const
        {
            resp.add(std::forward<ValueType>(val)).endl();
            return *this;
        }

        const ProtocolOut& ok() const
        {
            resp.add("OK\n", 3);
            return *this;
        }
        const ProtocolOut& fail() const
        {
            resp.add("FAIL\n", 5);
            return *this;
        }
        const ProtocolOut& ok(bool success) const
        {
            return success ? ok() : fail();
        }

        const ProtocolOut& endl() const
        {
            resp.endl();
            return *this;
        }

    private:
        const EvResponse &resp;
    };

}
