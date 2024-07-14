#!/bin/bash

if [[ $* == *--kernel* ]]; then
  qemu-system-i386 -kernel iso/os.iso
else
  qemu-system-i386 -cdrom iso/os.iso
fi
