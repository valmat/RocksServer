#!/bin/bash
# rocksserver

CUR_DIR="$(dirname "$0")"
cd $CUR_DIR
CUR_DIR="$(pwd)"

# Make dependences
../deps/make.sh

# Make sources
cd ../src
make clean
make static -j`nproc`
cd $CUR_DIR

##############################################
SRCDIR="../src"
rm -rf pkg

mkdir -p pkg/usr/local/bin
mkdir -p pkg/usr/lib/rocksserver/plugins
mkdir -p pkg/var/rocksserver
mkdir -p pkg/etc/rocksserver
mkdir -p pkg/var/log/rocksserver
mkdir -p pkg/etc/init.d
mkdir -p pkg/etc/cron.d
mkdir -p pkg/DEBIAN

cp "${SRCDIR}/bin/RocksServer.bin"    "pkg/usr/local/bin/rocksserver"
cp "${SRCDIR}/bin/restore.bin"        "pkg/usr/local/bin/rocksrestore"
cp "${SRCDIR}/bin/restore_hr.bin"     "pkg/usr/local/bin/rocksrestore_hr"
cp "${SRCDIR}/bin/human_readable.bin" "pkg/usr/local/bin/rocks_human_readable"
cp "${SRCDIR}/bin/rows_number.bin"    "pkg/usr/local/bin/rocks_rows_number"
cp "${SRCDIR}/bin/human_readable_batches.bin" "pkg/usr/local/bin/rocks_human_readable_batches"

cp "${SRCDIR}/config.ini" "pkg/etc/rocksserver/config.ini"
cp "scripts/initd"        "pkg/etc/init.d/rocksserver"
cp "scripts/crond"        "pkg/etc/cron.d/rocksserver"

##############################################
last_tag=$(git -C "." describe --tags --candidates=1)
version=${last_tag:1}
echo 
echo "version: [$version]"

SIZE=$(du -sb pkg/ | cut -f1)
SIZE=$(echo "$SIZE/1024" | bc)

cat control \
    | sed "s/%InstalledSize%/$SIZE/g" \
    | sed "s/%version%/$version/g" \
    > pkg/DEBIAN/control

##############################################
# configuration files list

echo "/etc/rocksserver/config.ini" > pkg/DEBIAN/conffiles
##############################################
cp scripts/preinst  pkg/DEBIAN/preinst
cp scripts/postinst pkg/DEBIAN/postinst
cp scripts/prerm    pkg/DEBIAN/prerm
cp scripts/postrm   pkg/DEBIAN/postrm
##############################################

fakeroot dpkg-deb --build pkg
mv pkg.deb "rocksserver_${version}_amd64.deb"
