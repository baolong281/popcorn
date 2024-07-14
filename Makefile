ASM=i686-elf-as
CC=i686-elf-gcc

SRC_DIR=src
BUILD_DIR=build
BOOT_DIR=iso


all: $(BUILD_DIR)/os.bin
	cp grub.cfg $(BOOT_DIR)/boot/grub/grub.cfg
	cp $(BUILD_DIR)/os.bin $(BOOT_DIR)/boot/os.bin
	grub-mkrescue -o $(BOOT_DIR)/os.iso $(BOOT_DIR)

$(BUILD_DIR)/os.bin: $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o
	$(CC) -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/os.bin -ffreestanding -O2 -nostdlib $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o -lgcc

$(BUILD_DIR)/boot.o: $(SRC_DIR)/boot.asm
	$(ASM) $(SRC_DIR)/boot.asm -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c
	$(CC) -c $(SRC_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

