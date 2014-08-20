#!/bin/bash

cd $(dirname $0)

#
# compile rocksdb
#
cd rocksdb
make
make static_lib
# make check

#
# compile libevent
#
cd ../libevent
./autogen.sh
./configure
make
#make verify
