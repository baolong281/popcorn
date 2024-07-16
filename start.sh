#!/bin/bash

# Initialize flags
kernel_mode=false
debug_mode=false

# Parse command line arguments
for arg in "$@"
do
    case $arg in
        --kernel)
        kernel_mode=true
        shift # Remove --kernel from processing
        ;;
        --debug)
        debug_mode=true
        shift # Remove --debug from processing
        ;;
    esac
done

# Construct the base command
if [ "$kernel_mode" = true ]; then
    cmd="qemu-system-i386 -kernel iso/os.iso"
else
    cmd="qemu-system-i386 -cdrom iso/os.iso"
fi

# Add debug options if --debug flag is present
if [ "$debug_mode" = true ]; then
    cmd+=" -s -S"
fi

# Execute the command
$cmd
