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


For more details see: [protocol description](protocol.md)

##Dependency
[RocksDB](https://github.com/facebook/rocksdb/), 
[LibEvent](http://libevent.org/)

The compiler should support `C++11`

## Run
First install libevend and compile or install RocksDB.
After run command 
```
make
```

After compiling run 

```
./RocksServer.bin config.ini
```

Or you can install it:
```
make
```
switch to root
```
make install
```

## Usage
Exemple of usage: [driver for PHP](drivers/php/README.md)
Or any your implementation by [protocol description](protocol.md).

## Note
Currently under development.

The RocksServer is workable but it needs some improvements.


Sorry for my english.

## License
[BSD](LICENSE)

