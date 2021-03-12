#!/bin/bash

set -e

# get the source code
git clone https://github.com/stevenj/avr-libc3 avr-libc3
cd avr-libc3
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
