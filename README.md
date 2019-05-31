# RocksServer is server for RocksDB


Supports the following operations:


* Get value / multi-values by key / keys
* Set value / multi-values by key / keys
* Delete key/keys from DB
* Check if key exists
* Imcrement value by key
* Itereate key-value pairs by key-prefix
* Itereate key-value pairs by seek key or key-prefix

For more details see: [protocol description](protocol.md)

##Dependency
[RocksDB](https://github.com/facebook/rocksdb/),
[LibEvent](http://libevent.org/)

The compiler should support `C++11`

## Install & Run
First install [RocksDB dependences](https://github.com/facebook/rocksdb/blob/master/INSTALL.md#dependencies):

```
sudo apt install libgflags-dev libsnappy-dev zlib1g-dev libbz2-dev liblz4-dev libzstd-dev
```


Then clone the repository:
```
git clone --recursive https://github.com/valmat/RocksServer
cd RocksServer
```
or
```
git clone https://github.com/valmat/RocksServer
cd RocksServer
git submodule update
```
After then compile dependencies:
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
Exemple of usage:
* [PHP driver](https://github.com/valmat/rocksdbphp)
* [Dlang driver](https://github.com/valmat/drocks) ([Dub package](https://code.dlang.org/packages/drocks))
* [Nodejs driver](https://github.com/valmat/queueServer/tree/master/client/node_rocks)

Or any your own implementation by [protocol description](protocol.md).


## How to extend
First you need implement your own request listener.
To do this, you should to implement the interface
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
