#!/bin/bash

set -e

OUTDIR="build"
BINVER="2.31.1"
GCCVER="8.2.0"
LIBVER="2.0.0"

# get the source code
wget https://sourceware.org/pub/binutils/releases/binutils-$BINVER.tar.xz
tar -xf  binutils-$BINVER.tar.xz
# build and install
mkdir -p binutils-$BINVER/$OUTDIR
cd       binutils-$BINVER/$OUTDIR
../configure          \
         --target=avr \
         --disable-nls
make -s
sudo make -s install
cd ../..
rm -rf   binutils-$BINVER

# get the source code
wget https://sourceware.org/pub/gcc/releases/gcc-$GCCVER/gcc-$GCCVER.tar.xz
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
         --disable-libssp         \
         --disable-libada         \
         --with-dwarf2            \
         --disable-shared         \
         --enable-static          \
         --disable-nls
make -s
sudo make -s install
cd ../..
rm -rf   gcc-$GCCVER

# get the source code
wget http://download.savannah.gnu.org/releases/avr-libc/avr-libc-$LIBVER.tar.bz2
tar -xf  avr-libc-$LIBVER.tar.bz2
# build and install
mkdir -p avr-libc-$LIBVER/$OUTDIR
cd       avr-libc-$LIBVER/$OUTDIR
../configure        \
         --host=avr \
         --build=`../avr-libc-$LIBVER/config.guess`
make -s
sudo make -s install
cd ../..
rm -rf   avr-libc-$LIBVER
