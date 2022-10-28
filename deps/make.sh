#!/bin/bash


cd $(dirname $0)

#
# compile libevent
#
cd libevent
./autogen.sh
./configure
make clean
make -j$(nproc) 
#make verify


#
# compile rocksdb
#
cd ../rocksdb
export LUA_PATH=
export LUA_INCLUDE=
make clean
make static_lib -j$(nproc)
#make shared_lib
# make check
