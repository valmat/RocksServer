 /**
 *  Protocol.h
 *
 *  Bridge to work with the message buffer.
 *  All messages go through the Protocol, but not on a straight line in the buffer.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class Protocol
    {
    public:
        Protocol(const EvResponse &r) :resp(r) {}
        Protocol(EvResponse &&r) :resp(std::move(r)) {}

        void setValue(const std::string &val) const
        {
            resp.add_printf("%lu\n%s", val.size(), val.c_str());
        }
        
        void setFailValue() const
        {
            resp.add("-1\n", 3);
        }

        void setStr(size_t val) const
        {
            resp.add_printf("%lu\n", val);
        }

        void ok() const
        {
            resp.add("OK\n", 3);
        }
        void fail() const
        {
            resp.add("FAIL\n", 5);
        }

        ~Protocol() {}
    private:
        const EvResponse &resp;
    };

}
