# ----- CONFIG -----
SRCDIR:=src
BOOTDIR:=boot
OBJDIR:=lib
# ----- END CONFIG -----

# IMPORTANT PATHS
# PLATFORM_TARGET=x86_64-w64-mingw32
PLATFORM_TARGET=x86_64-elf
GCC=$(shell pwd)/cross/bin/${PLATFORM_TARGET}-gcc
LD=$(shell pwd)/cross/bin/${PLATFORM_TARGET}-ld
OBJCOPY=$(shell pwd)/cross/bin/${PLATFORM_TARGET}-objcopy
OBJCOPY=/home/pam/opt/cross/bin/x86_64-elf-objcopy
GNUEFIDIR:=$(shell pwd)/gnu-efi

# BOOT_CFLAGS = -ffreestanding -I$(GNUEFIDIR)/inc -I$(GNUEFIDIR)/inc/x86_64 -I$(GNUEFIDIR)/inc/protocol
BOOT_CFLAGS = -I$(GNUEFIDIR)/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args
BOOT_LDFLAGS = -shared -Bsymbolic -L$(GNUEFIDIR)/x86_64/lib -L$(GNUEFIDIR)/x86_64/gnuefi -T$(GNUEFIDIR)/gnuefi/elf_x86_64_efi.lds $(GNUEFIDIR)/x86_64/gnuefi/crt0-efi-x86_64.o
# BOOT_LDFLAGS = -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main
BOOT_LIBS = -lgnuefi -lefi

boot: BOOTX64.EFI
iso: os.img

os.img: BOOTX64.EFI
	dd if=/dev/zero of=$@ bs=1k count=1440
	mformat -i $@ -f 1440 ::
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ $< ::/EFI/BOOT

clean:
	@rm -dfr $(OBJDIR)

BOOTX64.EFI: $(OBJDIR)/boot/main.so
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 $< $@
#	$(GCC) $(BOOT_LDFLAGS) -o $@ $^

# $(OBJDIR)/boot/data.o:
# 	@mkdir -p $(@D)
# 	$(GCC) $(BOOT_CFLAGS) -o $@ -c $(GNUEFIDIR)/lib/data.c

$(OBJDIR)/boot/main.so: $(OBJDIR)/boot/main.o
	$(LD) $(BOOT_LDFLAGS) $< -o $@ $(BOOT_LIBS)

$(OBJDIR)/boot/%.o: $(BOOTDIR)/%.c
	@mkdir -p $(@D)
	$(GCC) $(BOOT_CFLAGS) -o $@ -c $<