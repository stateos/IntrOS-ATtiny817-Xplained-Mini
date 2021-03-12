#!/bin/bash

set -e

COMMIT_LIBC="d09c2a61764aced3274b6dde4399e11b0aee4a87"

# get the source code
git clone https://github.com/stevenj/avr-libc3 avr-libc3
cd avr-libc3
git checkout $COMMIT_LIBC
./bootstrap
# build and install
mkdir -p build
cd build
../configure   \
	--host=avr \
	--build=`../avr-libc3/config.guess`
make -s
sudo make -s install
cd ../..
rm -fRd avr-libc3
