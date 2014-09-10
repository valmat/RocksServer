/**
 *  RequestStat.h
 *
 *  Request listener for command "stats" (Get statistics)
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestStat : public RequestBase
    {
    public:
        RequestStat(RocksDBWrapper &rdb) : _rdb(rdb) {}

        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &request, const EvBuffer &buf) override
        {
            std::string stat;
            if(_rdb->GetProperty("rocksdb.stats", &stat)) {
                buf.add(stat);
            } else {
                buf.add("FAIL", 4);
            }
        }

        virtual ~RequestStat() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
