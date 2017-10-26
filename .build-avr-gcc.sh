#!/bin/bash

set -e

OUTDIR = "build"

BINVER = "2.29"
# get the source code
wget     ftp://ftp.mirrorservice.org/sites/ftp.gnu.org/gnu/binutils/binutils-$BINVER.tar.xz
tar -xf  binutils-$BINVER.tar.xz
# build and install
mkdir -p binutils-$BINVER/$OUTDIR
cd       binutils-$BINVER/$OUTDIR
../configure          \
         --target=avr \
         --disable-nls
make -s
make -s  install
cd       ../..
rm -rf   binutils-$BINVER

GCCVER = "7.2.0"
# get the source code
wget     ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-$GCCVER/gcc-$GCCVER.tar.xz
tar -xf  gcc-$GCCVER.tar.xz
# download the prerequisites
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
make -s  install
cd       ../..
rm -rf   gcc-$GCCVER

LIBVER = "2.0.0"
# get the source code
wget     ftp://ftp.mirrorservice.org/sites/download.savannah.gnu.org/releases/avr-libc/avr-libc-$LIBVER.tar.bz2
tar -xf  avr-libc-$LIBVER.tar.bz2
# build and install
mkdir -p avr-libc-$LIBVER/$OUTDIR
cd       avr-libc-$LIBVER/$OUTDIR
../configure        \
         --host=avr \
         --build=`../avr-libc-$LIBVER/config.guess`
make -s
make -s  install
cd       ../..
rm -rf   avr-libc-$LIBVER
