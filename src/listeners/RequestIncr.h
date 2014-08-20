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
         *  @param       event buffer object
         */
        virtual void run(const EvRequest &request, const EvBuffer &buf) override
        {
            // Detect if current method is POST
            if( !request.isPost() ) {
                buf.add("Request method should be POST");
                return;
            }

            auto raw = request.getPostData();
            if(!raw.size()) {
                buf.add("FAIL", 4);
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
                buf.add("OK", 2);
            } else {
                buf.add("FAIL", 4);
            }
        }

        virtual ~RequestIncr() {}
    private:
        RocksDBWrapper& _rdb;
    };

}
