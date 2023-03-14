#!/usr/bin/env sh

# Get name of release from os-release (ID=something)
RELEASE=$(cat /etc/os-release | grep -E "^ID=.*" | cut -d "=" -f2-)


if [ $RELEASE == "ubuntu" ]; then
    sudo apt update && sudo apt install -y curl build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo mtools nasm
elif [ $RELEASE == "arch" ]; then
    sudo pacman -Syu base-devel gmp libmpc mpfr mtools nasm
else
    echo "Unknown distribution"
    exit 1
fi
