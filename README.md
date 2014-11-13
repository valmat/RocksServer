#RocksServer is server for RocksDB


Supports the following operations:


operations  | description
------------- | -------------
**Get** | Get value by key
**Multi get** | Get values by keys
**Set** | Set value by key
**Multi set** | Set values by keys
**Delete key** | Delete key from DB
**Multi delete keys** | Delete keys from DB
**Check key exist** | Check key existing
**Imcrement** | Imcrement value by key
**Prefit** | Prefix iterator


For more details see: [protocol description](protocol.md)

##Dependency
[RocksDB](https://github.com/facebook/rocksdb/), 
[LibEvent](http://libevent.org/)

The compiler should support `C++11`

## Install & Run
Clone the repository:
```
git clone --recursive git@github.com:valmat/RocksServer.git
cd RocksServer
```
or
```
git clone git@github.com:valmat/RocksServer.git
cd RocksServer
git submodule update
```
Then compile dependencies:
```
./deps/make.sh
```


Compile:
```
cd src
make
```

After:
edit config.ini and run

```
./RocksServer.bin config.ini
```

Or you can install it:

switch to root and run
```
make install
```

## Usage
Exemple of usage: [driver for php](https://github.com/valmat/rocksdbphp)
Or any your own implementation by [protocol description](protocol.md).


## How to extend
First you need implement your own request listener.
To do this, you have to implement the interface
```c
template<typename ProtIn, typename ProtOut>
struct RequestBase: public RequestSuperBase
{
    virtual void run(const ProtIn &, const ProtOut &) = 0;
};
```

For example:
`RequestPing.h`
```c
struct RequestPing : public RequestBase<ProtocolInTrivial, ProtocolOut>
{
    virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override
    {
        out.setStr("pong");
    }
};
```
After that, you need to implement a plugin:
`smpl_plug.cpp`
```c
#include <rocksserver/api.h>
#include "RequestPing.h"

PLUGIN(Extension &extension)
{
    extension.bind("/ping", new RequestPing());
}
```
Compile your plugin:
```
g++ -I. -I"/usr/include/rocksserver/include" -Wall -O3 -std=c++11 -fPIC -c smpl_plug.cpp -o smpl_plug.o 
g++ -Wall -std=c++11 -O3 -shared smpl_plug.o -o smpl_plug.so
```
and copy to `/usr/lib/rocksserver/plugins`
```
cp -f smpl_plug.so /usr/lib/rocksserver/plugins/smpl_plug.so
```
Restart RocksServer.
See [example](https://github.com/valmat/RocksServer/tree/master/extension_example) how to extend RocksServer.

## How to customize
Just edit [config.ini](https://github.com/valmat/RocksServer/blob/master/src/config.ini)

## License
[BSD](LICENSE)

