#!/bin/bash

# Check if the script is run as root
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root"
    exit
fi

filename=$1 # First argument is the file name
blockSize=512 # Start with 512 bytes

while [ $blockSize -le 4194304 ]; do
    # Clearing cache
    echo 3 > /proc/sys/vm/drop_caches
    echo "Block Size: $blockSize bytes (without cache)"
    ./run3 $filename $blockSize

    echo "Block Size: $blockSize bytes (with cache)"
    ./run3 $filename $blockSize

    blockSize=$(( $blockSize * 2 )) # Double the block size
done
