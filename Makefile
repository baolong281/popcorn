# ASM=i686-elf-as
# CC=i686-elf-gcc
#
# KERNEL_DIR=kernel
# BUILD_DIR=build
# BOOT_DIR=iso
#
# C_SOURCE=$(wildcard $(KERNEL_DIR)/*.c)
# ASM_SOURCE=$(wildcard $(KERNEL_DIR)/*.asm)
#
# OBJS=$(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES)) \
#      $(patsubst $(KERNEL_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))
#
#
# all: $(BUILD_DIR)/os.bin
# 	cp $(BUILD_DIR)/os.bin $(BOOT_DIR)/boot/os.bin
# 	grub-mkrescue -o $(BOOT_DIR)/os.iso $(BOOT_DIR)
#
# kernel:
#
# $(BUILD_DIR)/os.bin: $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o
# 	$(CC) -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/os.bin -ffreestanding -O2 -nostdlib $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o -lgcc
#
# $(BUILD_DIR)/boot.o: $(SRC_DIR)/boot.asm
# 	$(ASM) $(SRC_DIR)/boot.asm -o $(BUILD_DIR)/boot.o
#
# $(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c
# 	$(CC) -c $(SRC_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
#
#
ASM=i686-elf-as
CC=i686-elf-gcc

KERNEL_DIR=kernel
BUILD_DIR=build
BOOT_DIR=iso

C_SOURCES=$(wildcard $(KERNEL_DIR)/*.c)
ASM_SOURCES=$(wildcard $(KERNEL_DIR)/*.asm)

OBJS=$(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES)) \
     $(patsubst $(KERNEL_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

all: $(BUILD_DIR)/os.bin
	cp $(BUILD_DIR)/os.bin $(BOOT_DIR)/boot/os.bin
	grub-mkrescue -o $(BOOT_DIR)/os.iso $(BOOT_DIR)
	grub-file --is-x86-multiboot $(BOOT_DIR)/os.iso

$(BUILD_DIR)/os.bin: $(OBJS)
	$(CC) -T $(KERNEL_DIR)/linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm
	$(ASM) $< -o $@

iso: $(BUILD_DIR)/os.bin
	cp $(BUILD_DIR)/os.bin $(BOOT_DIR)/boot/os.bin
	grub-mkrescue -o $(BOOT_DIR)/os.iso $(BOOT_DIR)

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/os.bin

.PHONY: all clean iso
