# x86 is default target
CPU_ARCH ?= -D__X86_ARCH_TARGET

# compiler variables
CC := gcc
CFLAGS := -m32 -no-pie -fno-stack-protector -fno-builtin -nostdinc -nostdlib \
	-nostartfiles -nodefaultlibs -Wall -Wextra -c $(CPU_ARCH)

# assembler variables
ASM := nasm
ASMFLAGS := -f elf32

# static linker variables
LD := ld
LDFLAGS := -T link.ld -melf_i386

# kernel source files
KCSOURCES := $(wildcard src/kernel/*.c)
KCSOURCES += $(wildcard src/drivers/*.c)
KCSOURCES += $(wildcard src/lib/*.c)
KASMSOURCES := $(wildcard src/kernel/*.s)

# kernel object files
KOBJECTS := $(patsubst %.c, %.o, $(KCSOURCES))
KOBJECTS += $(patsubst %.s, %.o, $(KASMSOURCES))

.PHONY: kernel.iso run clean

.DEFAULT_GOAL := kernel.iso

$(KOBJECTS):
	$(MAKE) -C src/kernel/ CC="$(CC)" CFLAGS="$(CFLAGS)" \
		ASM="$(ASM)" ASMFLAGS="$(ASMFLAGS)"
	$(MAKE) -C src/drivers/ CC="$(CC)" CFLAGS="$(CFLAGS)"
	$(MAKE) -C src/lib/ CC="$(CC)" CFLAGS="$(CFLAGS)"

kentry.o: kentry.s
	$(ASM) $(ASMFLAGS) kentry.s


kernel.elf: $(KOBJECTS) kentry.o
	echo $(KSOURCES)
	$(LD) $(LDFLAGS) $(KOBJECTS) kentry.o -o kernel.elf

kernel.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                   \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot                \
		-boot-load-size 4            \
		-A os                        \
		-input-charset utf8          \
		-quiet                       \
		-boot-info-table             \
		-o kernel.iso                \
		iso

run: kernel.iso
	qemu-system-i386                                  \
		-accel tcg,thread=single                      \
		-cpu core2duo                                 \
		-m 128                                        \
		-no-reboot                                    \
		-drive format=raw,media=cdrom,file=kernel.iso \
		-serial stdio                                 \
		-smp 1                                        \
		-usb                                          \
		-vga std


clean:
	$(MAKE) -C src/kernel/ clean
	$(MAKE) -C src/drivers/ clean
	rm -rf kernel.elf kernel.iso iso/boot/kernel.elf

