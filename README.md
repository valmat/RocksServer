#Simple, but flexible and fast server for RocksDB

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
Exemple of usage: [driver for PHP](https://github.com/valmat/rocksdbphp)
Or any your own implementation by [protocol description](protocol.md).


## License
[BSD](LICENSE)

