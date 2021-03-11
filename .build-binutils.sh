#!/bin/bash

set -e

BINVER="2.36.1"

# get the source code
wget https://ftpmirror.gnu.org/binutils/binutils-$BINVER.tar.xz
tar -xf  binutils-$BINVER.tar.xz
# build and install
mkdir -p binutils-$BINVER/build
cd binutils-$BINVER/build
../configure       \
	--target=avr   \
	--disable-nls  \
	--disable-werror
make -s
sudo make -s install
cd ../..
rm -fRd binutils-$BINVER
