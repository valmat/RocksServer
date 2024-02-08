# RocksServer: A Server for RocksDB

RocksServer supports the following operations:

* Get a value or multiple values by key or keys
* Set a value or multiple values by key or keys
* Delete key or keys from the database
* Check if a key exists
* Increment a value by key
* Iterate over key-value pairs by key prefix
* Iterate over key-value pairs by seek key or key prefix

For more details, see: [protocol description](protocol.md)

## Dependency
RocksServer depends on [RocksDB](https://github.com/facebook/rocksdb/) and [LibEvent](http://libevent.org/).

The compiler should support `C++11`.

## Install & Run

First, install [RocksDB dependencies](https://github.com/facebook/rocksdb/blob/master/INSTALL.md#dependencies):

```sh
sudo apt install libgflags-dev libsnappy-dev zlib1g-dev libbz2-dev liblz4-dev libzstd-dev
```

Then clone the repository:

```sh
git clone --recursive https://github.com/valmat/RocksServer
cd RocksServer
```

Or:

```sh
git clone https://github.com/valmat/RocksServer
cd RocksServer
git submodule update --init
```

After that, compile the dependencies:

```sh
./deps/make.sh
```

Compile the server:

```sh
cd src
make
```

Then, edit `config.ini` and run:

```sh
./RocksServer.bin config.ini
```

Alternatively, you can install it:

Switch to root and run:

```sh
make install
```

## Usage

Examples of usage:
* [PHP driver](https://github.com/valmat/rocksdbphp)
* [Dlang driver](https://github.com/valmat/drocks) ([Dub package](https://code.dlang.org/packages/drocks))
* [Node.js driver](https://github.com/valmat/queueServer/tree/master/client/node_rocks)

You can also implement your own client based on the [protocol description](protocol.md).

## How to Extend

To extend RocksServer, you should first implement your own request listener by implementing the following interface:

```cpp
template<typename ProtIn, typename ProtOut>
struct RequestBase: public RequestSuperBase
{
    virtual void run(const ProtIn &, const ProtOut &) = 0;
};
```

Example:

`RequestPing.h`

```cpp
struct RequestPing : public RequestBase<ProtocolInTrivial, ProtocolOut>
{
    virtual void run(const ProtocolInTrivial &in, const ProtocolOut &out) override
    {
        out.setStr("pong");
    }
};
```

Then, implement a plugin:

`smpl_plug.cpp`

```cpp
#include <rocksserver/api.h>
#include "RequestPing.h"

PLUGIN(Extension &extension)
{
    extension.bind("/ping", new RequestPing());
}
```

Compile your plugin:

```sh
g++ -I. -I"/usr/include/rocksserver/include" -Wall -O3 -std=c++11 -fPIC -c smpl_plug.cpp -o smpl_plug.o
g++ -Wall -std=c++11 -O3 -shared smpl_plug.o -o smpl_plug.so
```

Copy it to `/usr/lib/rocksserver/plugins`:

```sh
cp -f smpl_plug.so /usr/lib/rocksserver/plugins/smpl_plug.so
```

Restart RocksServer. See this [example](https://github.com/valmat/RocksServer/tree/master/extension_example) for how to extend RocksServer.

## How to Customize

To customize RocksServer, edit [config.ini](https://github.com/valmat/RocksServer/blob/master/src/config.ini).

## License

RocksServer is licensed under the [BSD License](LICENSE).