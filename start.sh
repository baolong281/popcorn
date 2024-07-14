#!/bin/bash

if [[ $* == *--grub* ]]; then
  qemu-system-i386 -cdrom iso/os.iso
else
  qemu-system-i386 -kernel iso/os.iso
fi
