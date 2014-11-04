/**
 *
 *  ExtensionObj.h
 *  Container for RocksServer Extension
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

namespace RocksServer {

    struct ExtensionObj
    {
        ExtensionObj(std::string &&path, RequestSuperBase* &&listener) :
            path(path),
            listener(listener)
        {}

    //private:
        std::string path;
        RequestSuperBase * listener;
    };

}