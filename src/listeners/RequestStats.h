/**
 *  RequestStats.h
 *
 *  Request listener for command "stats" (Get statistics)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestStats : public RequestBase
    {
    public:
        RequestStats(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       protocol object
         */
        virtual void run(const EvRequest &request, const Protocol &prot) override
        {
            std::string stat;
            if(_rdb->GetProperty("rocksdb.stats", &stat)) {
                prot.setStr(stat);
            } else {
                prot.fail();
                EvLogger::writeLog(_rdb.getStatus().data());
            }
        }

        virtual ~RequestStats() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
