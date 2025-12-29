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

    template<class T>
    struct is_optional : std::false_type {};

    template<class U>
    struct is_optional<std::optional<U>> : std::true_type {};

    template<class T>
    inline constexpr bool is_optional_v = is_optional<std::remove_cvref_t<T>>::value;


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

        const ProtocolOut& setStr() const
        {
            return *this;
        }

        // optional
        template<class Opt>
            requires is_optional_v<Opt>
        const ProtocolOut& setStr(Opt&& opt) const
        {
            if (!opt) {return setStr();}
            return setStr(std::forward<Opt>(opt).value());
        }

        template<class... Opt>
            requires (is_optional_v<Opt> && ...)
        const ProtocolOut& setStr(Opt&&... opt) const
        {
            if (!(... || static_cast<bool>(opt)))
                return setStr();

            ((opt ? (void)setStr(std::forward<Opt>(opt).value())
                  : (void)setStr()),
             ...);

            return *this;
        }

        template<typename KeyType, class Opt>
            requires is_optional_v<Opt>
        const ProtocolOut& setPair(const KeyType& key, Opt&& opt) const
        {
            if (opt) {
                return setPair(key, std::forward<Opt>(opt).value());
            }

            return setFailPair(key);
        }

        template<class Opt>
            requires is_optional_v<Opt>
        const ProtocolOut& setValue(Opt&& opt) const
        {
            if (opt) {
                return setValue(std::forward<Opt>(opt).value());
            }

            return setFailValue();
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

        const ProtocolOut& setCode(int code, const char* reason) const
        {
            resp.set_code(code, reason);
            return *this;
        }
        const ProtocolOut& setCode(int code) const
        {
            resp.set_code(code);
            return *this;
        }

    private:
        const EvResponse &resp;
    };

}
