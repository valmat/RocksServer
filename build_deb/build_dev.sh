#!/bin/bash
# rocksserver-dev

CUR_DIR="$(dirname "$0")"
cd $CUR_DIR
CUR_DIR="$(pwd)"


##############################################
SRCDIR="../src"
rm -rf pkg

mkdir -p pkg/usr/include/rocksserver
mkdir -p pkg/usr/include/rocksserver/rocksdb
mkdir -p pkg/DEBIAN

cp -r ${SRCDIR}/include/*                 pkg/usr/include/rocksserver/
cp -r ${SRCDIR}/../deps/rocksdb/include/* pkg/usr/include/rocksserver/rocksdb/

##############################################
last_tag=$(git -C "." describe --tags --candidates=1)
version=${last_tag:1}
echo 
echo "version: [$version]"

SIZE=$(du -sb pkg/ | cut -f1)
SIZE=$(echo "$SIZE/1024" | bc)

cat control_dev \
    | sed "s/%InstalledSize%/$SIZE/g" \
    | sed "s/%version%/$version/g" \
    > pkg/DEBIAN/control

##############################################

fakeroot dpkg-deb --build pkg
mv pkg.deb "rocksserver-dev_${version}_amd64.deb"
