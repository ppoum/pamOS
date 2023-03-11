#!/usr/bin/env sh

# CHANGE THOSE VALUES IF WANTED
export MAKE_CPU_CORES=20
BINUTILS_SRC_URL="https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.gz"
GDB_SRC_URL="https://ftp.gnu.org/gnu/gdb/gdb-13.1.tar.gz"
GCC_SRC_URL="https://ftp.gnu.org/gnu/gcc/gcc-12.2.0/gcc-12.2.0.tar.gz"

# Remove previous toolchain if present, recompile all
rm -dfr cross
mkdir -p cc-src

echo "----------"
echo "INSTALLING DEPENDENCIES"
echo "----------"
sh scripts/install-deps.sh


echo "----------"
echo "DOWNLOADING CROSS-COMPILER SOURCES"
echo "----------"
if [ -d "cc-src/binutils" ]; then
    echo "binutils already downloaded, skippping."
else
    echo "Downloading binutils"
    curl $BINUTILS_SRC_URL -o cc-src/binutils.tar.gz
    mkdir -p cc-src/binutils
    tar -zxf cc-src/binutils.tar.gz -C cc-src/binutils --strip-components=1
    rm cc-src/binutils.tar.gz
fi

if [ -d "cc-src/gdb" ]; then
    echo "gdb already downloaded, skippping."
else
    echo "Downloading gdb"
    curl $GDB_SRC_URL -o cc-src/gdb.tar.gz
    mkdir -p cc-src/gdb
    tar -zxf cc-src/gdb.tar.gz -C cc-src/gdb --strip-components=1
    rm cc-src/gdb.tar.gz
fi

if [ -d "cc-src/gcc" ]; then
    echo "gcc already downloaded, skippping."
else
    echo "Downloading gcc"
    curl $GCC_SRC_URL -o cc-src/gcc.tar.gz
    mkdir -p cc-src/gcc
    tar -zxf cc-src/gcc.tar.gz -C cc-src/gcc --strip-components=1
    rm cc-src/gcc.tar.gz
fi


echo "----------"
echo "BUILDING BINUTILS"
echo "----------"
# Variables set here are used for every cross-compiler related build
export PREFIX="$(pwd)/cross"
export TARGET=x86_64-elf
export TARGETS=$TARGET,x86_64-pe
export PATH="$PREFIX/bin:$PATH"
sh scripts/build-binutils.sh
# Also build binutils for windows to have an objcopy with pei-x86_64 target
# (Build fails with --enable-targets=pei-x86_64)
# export TARGET=x86_64-w64-mingw32
# sh scripts/build-binutils.sh

echo "----------"
echo "BUILDING GDB"
echo "----------"
sh scripts/build-gdb.sh

echo "----------"
echo "BUILDING GCC"
echo "----------"
sh scripts/build-gcc.sh

echo "----------"
echo "DOWNLOADING ADDITIONAL DEPENDENCIES"
echo "----------"
sh scripts/build-gnu-efi.sh

echo "----------"
echo "COPYING LINUX HEADER FILES"
echo "----------"
mkdir -p inc
cp /usr/aarch64-linux-gnu/include/asm-generic/int-ll64.h inc/int-ll64.h
# Remove unused import
sed -i '/.*bitsperlong.*/d' inc/int-ll64.h

cp /usr/include/linux/elf.h inc/elf.h
# Replace import to linux/types.h to int-ll64.h, as it is the only type used
sed -i inc/elf.h -e '/.*types.h>/d' -e 's/linux\/elf-em/int-ll64/'

echo "----------"
echo "CLEANING UP"
echo "----------"
rm -dfr cc-src/build-binutils cc-src/build-gdb cc-src/build-gcc

echo "----------"
echo "DONE!"
echo "----------"