/**
 *  RequestIncr.h
 *
 *  Request listener for command "/incr"
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class RequestIncr : public RequestBase
    {
    public:
        RequestIncr(RocksDBWrapper &rdb) : _rdb(rdb) {}
        
        /**
         *  Runs request listener
         *  @param       event request object
         *  @param       protocol object
         */
        virtual void run(const EvRequest &request, const Protocol &prot) override
        {
            // Detect if current method is POST
            if( !request.isPost() ) {
                EvLogger::writeLog(EvLogger::Level::warn, "Request method should be POST");
                prot.fail();
                return;
            }

            auto raw = request.getPostData();
            if(!raw.size()) {
                prot.fail();
                return;
            }

            bool rez;
            auto seppos = raw.find('&');
            auto len = raw.size();

            if( seppos < len-1 ) {
                rez = _rdb.incr( rocksdb::Slice( raw, seppos ),  rocksdb::Slice( raw+seppos+1, len - seppos - 1 ) );
            } else {
                rez = _rdb.incr( rocksdb::Slice(raw, raw.size()), "1" );
            }
            
            if( rez ) {
                prot.ok();
            } else {
                prot.fail();
                EvLogger::writeLog(EvLogger::Level::warn, _rdb.getStatus().data());
            }
        }

        virtual ~RequestIncr() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
