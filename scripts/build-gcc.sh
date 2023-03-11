#!/usr/bin/env sh

cd cc-src

# Delete build dir if it exists, recreate empty
rm -dfr build-gcc
mkdir -p build-gcc

cd build-gcc
../gcc/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make -j$MAKE_CPU_CORES all-gcc
make -j$MAKE_CPU_CORES all-target-libgcc
make -j$MAKE_CPU_CORES install-gcc
make -j$MAKE_CPU_CORES install-target-libgcc
