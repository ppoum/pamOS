#!/usr/bin/env sh

qemu-system-x86_64 -L /usr/share/ovmf/ -drive if=pflash,file=/usr/share/ovmf/OVMF.fd,format=raw -cdrom os.img