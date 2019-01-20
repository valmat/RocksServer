#!/bin/bash

cd $(dirname $0)

#
# compile libevent
#
cd libevent
./autogen.sh
./configure
make
#make verify


#
# compile rocksdb
#
cd ../rocksdb
export LUA_PATH=
export LUA_INCLUDE=
make clean
make static_lib
#make shared_lib
# make check
