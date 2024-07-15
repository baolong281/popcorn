ASM=i686-elf-as
CC=i686-elf-gcc

KERNEL_DIR=kernel
LIBC_DIR=libc
SYSROOT_DIR=sysroot
BOOT_DIR=$(SYSROOT_DIR)/usr/boot
ISO_DIR=iso

LIBC_HEADERS=$(LIBC_DIR)/include
KERNEL_HEADERS=$(KERNEL_DIR)/include

C_SOURCES=$(wildcard $(KERNEL_DIR)/*.c)
ASM_SOURCES=$(wildcard $(KERNEL_DIR)/*.asm)
LIBC_SOURCES=$(wildcard $(LIBC_DIR)/*.c)

OBJS=$(patsubst $(KERNEL_DIR)/%.c, $(BOOT_DIR)/%.o, $(C_SOURCES)) \
     $(patsubst $(KERNEL_DIR)/%.asm, $(BOOT_DIR)/%.o, $(ASM_SOURCES))

LIBC_OBJS=$(patsubst $(LIBC_DIR)/%.c, $(SYSROOT_DIR)/usr/lib/%.o, $(LIBC_SOURCES))

build: all
	mkdir -p $(ISO_DIR)
	mkdir -p $(ISO_DIR)/boot
	mkdir -p $(ISO_DIR)/boot/grub

	cp $(BOOT_DIR)/os.bin $(ISO_DIR)/boot/os.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg

	grub-mkrescue -o $(ISO_DIR)/os.iso $(ISO_DIR)

all: headers $(BOOT_DIR)/os.bin libc

$(BOOT_DIR)/os.bin: $(OBJS) $(LIBC_OBJS)
	$(CC) -T $(KERNEL_DIR)/linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

$(BOOT_DIR)/%.o: $(KERNEL_DIR)/%.c 
	mkdir -p $(SYSROOT_DIR)/usr/boot
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -isystem ./$(SYSROOT_DIR)/usr/include

$(SYSROOT_DIR)/usr/boot/%.o: $(KERNEL_DIR)/%.asm
	mkdir -p $(SYSROOT_DIR)/usr/boot
	$(ASM) $< -o $@

$(SYSROOT_DIR)/usr/lib/%.o: $(LIBC_DIR)/%.c
	mkdir -p $(SYSROOT_DIR)/usr/lib
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -isystem ./$(SYSROOT_DIR)/usr/include

libc: headers $(LIBC_OBJS)

headers: $(LIBC_HEADERS) $(KERNEL_HEADERS)
	mkdir -p $(SYSROOT_DIR)/usr/include
	cp -r $(LIBC_DIR)/include/* $(SYSROOT_DIR)/usr/include
	cp -r $(KERNEL_HEADERS) $(SYSROOT_DIR)/usr/include/kernel

clean:
	rm -f $(BOOT_DIR)/*.o $(BOOT_DIR)/os.bin
	rm -rf $(SYSROOT_DIR)/usr/lib/*.o $(SYSROOT_DIR)/usr/include/*
	rm -rf $(ISO_DIR)

.PHONY: all clean iso libc headers

