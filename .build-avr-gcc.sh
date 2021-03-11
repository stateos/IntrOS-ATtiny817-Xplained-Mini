#!/bin/bash

set -e

OUTDIR="build"
BINVER="2.36.1"
GCCVER="10.2.0"
COMMIT_LIBC="d09c2a61764aced3274b6dde4399e11b0aee4a87"

# install dependencies
apt-get -y -qq install wget make mingw-w64 gcc g++ bzip2 git autoconf texinfo

# get the source code
wget https://ftpmirror.gnu.org/binutils/binutils-$BINVER.tar.xz
tar -xf  binutils-$BINVER.tar.xz
# build and install
mkdir -p binutils-$BINVER/$OUTDIR
cd       binutils-$BINVER/$OUTDIR
../configure          \
	   --target=avr   \
	   --disable-nls  \
	   --disable-werror
make -s
sudo make -s install
cd ../..
rm -rf binutils-$BINVER

# get the source code
wget https://ftpmirror.gnu.org/gcc/gcc-$GCCVER/gcc-$GCCVER.tar.xz
tar -xf  gcc-$GCCVER.tar.xz
# download prerequisites
cd       gcc-$GCCVER
./contrib/download_prerequisites
# build and install
mkdir -p $OUTDIR
cd       $OUTDIR
../configure                      \
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
rm -rf gcc-$GCCVER

# get the source code
git clone https://github.com/stevenj/avr-libc3 avr-libc3
cd avr-libc3
git checkout $COMMIT_LIBC
./bootstrap
mkdir -p $OUTDIR
cd       $OUTDIR
../configure        \
         --host=avr \
         --build=`../avr-libc3/config.guess`
make -s
sudo make -s install
cd ../..
rm -rf avr-libc3
