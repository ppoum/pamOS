#!/usr/bin/env sh

cd cc-src

# Delete build dir if it exists, recreate empty
rm -dfr build-gdb
mkdir -p build-gdb

cd build-gdb
../gdb/configure --target=$TARGET --prefix="$PREFIX" --disable-werror
make -j$MAKE_CPU_CORES all-gdb
make -j$MAKE_CPU_CORES install-gdb
