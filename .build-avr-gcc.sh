#!/bin/bash

set -e

GCCVER="10.2.0"

# get the source code
wget https://ftpmirror.gnu.org/gcc/gcc-$GCCVER/gcc-$GCCVER.tar.xz
tar -xf  gcc-$GCCVER.tar.xz
# download prerequisites
cd gcc-$GCCVER
./contrib/download_prerequisites
# build and install
mkdir -p build
cd build
../configure                 \
	--target=avr             \
	--enable-languages=c,c++ \
	--disable-nls            \
	--disable-libssp         \
	--disable-libada         \
	--with-dwarf2            \
	--disable-shared         \
	--enable-static          \
	--enable-mingw-wildcard  \
	--enable-plugin          \
	--with-gnu-as
make -s
sudo make -s install
cd ../..
rm -fRd gcc-$GCCVER
