#!/usr/bin/env sh

cd cc-src

# Delete build dir if it exists, recreate empty
rm -dfr build-binutils
mkdir -p build-binutils

cd build-binutils
../binutils/configure --target=$TARGET --enable-targets=$TARGETS --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$MAKE_CPU_CORES
make -j$MAKE_CPU_CORES install
