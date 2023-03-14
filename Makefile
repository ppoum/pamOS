# Recursive wildcard - $(call rwildcard, DIR, PATTERN)
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# ----- CONFIG -----
SRCDIR:=src
BOOTDIR:=boot
OBJDIR:=lib
# ----- END CONFIG -----

# IMPORTANT PATHS
PLATFORM_TARGET=x86_64-elf
GCC=$(shell pwd)/cross/bin/${PLATFORM_TARGET}-gcc
LD=$(shell pwd)/cross/bin/${PLATFORM_TARGET}-ld
OBJCOPY=$(shell pwd)/cross/bin/${PLATFORM_TARGET}-objcopy
GNUEFIDIR:=$(shell pwd)/gnu-efi

BOOT_CFLAGS = -I$(GNUEFIDIR)/inc -Iinc/ -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar
BOOT_CFLAGS += -mno-red-zone -maccumulate-outgoing-args
BOOT_LDFLAGS = -shared -Bsymbolic -L$(GNUEFIDIR)/x86_64/lib -L$(GNUEFIDIR)/x86_64/gnuefi -T$(GNUEFIDIR)/gnuefi/elf_x86_64_efi.lds 
BOOT_LDFLAGS += $(GNUEFIDIR)/x86_64/gnuefi/crt0-efi-x86_64.o
BOOT_LIBS = -lgnuefi -lefi

CFLAGS = -ffreestanding -mno-red-zone -nostdlib -g -Wall


# Kernel sources (used after exiting EFI)
C_SRCS = $(call rwildcard, $(SRCDIR), *.c)
C_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(C_SRCS))
ASM_SRCS = $(call rwildcard, $(SRCDIR), *.asm)
ASM_OBJS = $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%.o, $(ASM_SRCS))

# ----- ROOT TARGETS -----
all: iso
boot: BOOTX64.EFI
iso: os.img

qemu: os.img
	$(shell ./scripts/start-qemu.sh)

debug: os.img
	$(shell ./scripts/start-qemu-debug.sh)
# ----- END ROOT TARGETS -----

os.img: BOOTX64.EFI $(OBJDIR)/kernel.elf
	dd if=/dev/zero of=$@ bs=1k count=1440
	mformat -i $@ -f 1440 ::
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ $< ::/EFI/BOOT
	mcopy -i $@ $(OBJDIR)/kernel.elf ::

clean:
	@rm -dfr $(OBJDIR)

# Top EFI target
BOOTX64.EFI: $(OBJDIR)/boot/main.so
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 $< $@

# Top ELF target
$(OBJDIR)/kernel.elf: $(C_OBJS) $(ASM_OBJS) $(OBJDIR)/etc/font.o
	$(LD) -o $@ $^ -e main

# Boot (EFI) related dependencies
$(OBJDIR)/boot/main.so: $(OBJDIR)/boot/main.o
	$(LD) $(BOOT_LDFLAGS) $< -o $@ $(BOOT_LIBS)

$(OBJDIR)/boot/%.o: $(BOOTDIR)/%.c
	@mkdir -p $(@D)
	$(GCC) $(BOOT_CFLAGS) -o $@ -c $<

# Kernel (ELF) related dependencies
#  - C files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(GCC) $(CFLAGS) -o $@ -c $<

#  - Asm files
$(OBJDIR)/%.o: $(SRCDIR)/%.asm
	@mkdir -p $(@D)
	nasm $< -f elf64 -o $@

#  - Font used by stdio (copy to binary object with label usable by other modules)
$(OBJDIR)/etc/font.o: $(SRCDIR)/etc/font.psf
	@mkdir -p $(@D)
	cd $(SRCDIR)/etc; $(OBJCOPY) -O elf64-x86-64 -B i386 -I binary font.psf ../../$(OBJDIR)/etc/font.o