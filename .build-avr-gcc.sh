#! /bin/bash

set -e

SRCDIR="source"
BINDIR="build"
INSTALL_DIR="/usr"

rm -rf   $SRCDIR

BINUTILS_VERSION="2.29"

# get the source code
mkdir -p $SRCDIR/$BINDIR
cd       $SRCDIR
wget     ftp://ftp.mirrorservice.org/sites/ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.xz
tar -xf  binutils-$BINUTILS_VERSION.tar.xz
# create the build directory
cd       $BINDIR
# build
../binutils-$BINUTILS_VERSION/configure \
         --prefix=$INSTALL_DIR          \
         --target=avr                   \
         --disable-nls
make -s
make -s  install
cd       ../..
rm -rf   $SRCDIR

GCC_VERSION="7.2.0"

# get the source code
mkdir -p $SRCDIR/$BINDIR
cd       $SRCDIR
wget     ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz
tar -xf  gcc-$GCC_VERSION.tar.xz
# download the prerequisites
cd       gcc-${GCC_VERSION}
./contrib/download_prerequisites
# create the build directory
cd       ..
cd       $BINDIR
# build
../gcc-$GCC_VERSION/configure     \
         --prefix=$INSTALL_DIR    \
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
rm -rf   $SRCDIR

LIBC_VERSION="2.0.0"

# get the source code
mkdir -p $SRCDIR/$BINDIR
cd       $SRCDIR
wget     ftp://ftp.mirrorservice.org/sites/download.savannah.gnu.org/releases/avr-libc/avr-libc-$LIBC_VERSION.tar.bz2
tar -xf  avr-libc-$LIBC_VERSION.tar.bz2
# create the build directory
cd       $BINDIR
# build
../avr-libc-$LIBC_VERSION/configure \
         --prefix=$INSTALL_DIR      \
         --host=avr                 \
         --build=`../avr-libc-$LIBC_VERSION/config.guess`
make -s
make -s  install
cd       ../..
rm -rf   $SRCDIR
