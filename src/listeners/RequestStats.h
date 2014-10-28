/**
 *  RequestStats.h
 *
 *  Request listener for command "stats" (Get statistics)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestStats : public RequestBase<ProtocolInTrivial, ProtocolOut>
    {
    public:
        RequestStats(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       protocol in object
         *  @param       protocol out object
         */
        virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override
        {
            std::string stat;
            if(_rdb->GetProperty("rocksdb.stats", &stat)) {
                out.setStr(stat);
            } else {
                out.fail();
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestStats() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
