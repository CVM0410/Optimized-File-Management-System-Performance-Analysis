#!/bin/bash

FILE_SIZES_MB=(1 10 100 1000) # File sizes in megabytes
BLOCK_SIZE=4096 # Fixed block size for copying

# Function to create a file of a specific size with random data
create_test_file() {
    local file_size_mb=$1
    local file_name="fileA_${file_size_mb}MB"
    dd if=/dev/urandom of="$file_name" bs=1M count=$file_size_mb 2>/dev/null
    echo "$file_name"
}

# Function to run your program for copying
run_your_program() {
    local input_file=$1
    local output_file="fileB_${input_file}"
    local file_size_mb=$(echo "$input_file" | grep -o -E '[0-9]+')

    echo "Running your program for copying $input_file to $output_file..."
    local start_time=$(date +%s.%N)
    ./runtest "$input_file" "$output_file" $BLOCK_SIZE
    local end_time=$(date +%s.%N)

    local duration=$(echo "$end_time - $start_time" | bc)
    local speed=$(echo "$file_size_mb / $duration" | bc -l)
    printf "Duration: %.3f seconds, Speed: %.3f MiB/s\n" "$duration" "$speed"
}

# Function to run dd for copying
run_dd() {
    local input_file=$1
    local output_file="fileB_dd_${input_file}"
    local file_size_mb=$(echo "$input_file" | grep -o -E '[0-9]+')

    echo "Running dd for copying $input_file to $output_file..."
    local start_time=$(date +%s.%N)
    dd if="$input_file" of="$output_file" bs=$BLOCK_SIZE 2>&1 | grep -Eo '[0-9]+ bytes \(.+?\) copied, .+? s, .+?'
    local end_time=$(date +%s.%N)

    local duration=$(echo "$end_time - $start_time" | bc)
    local speed=$(echo "$file_size_mb / $duration" | bc -l)
    printf "Duration: %.3f seconds, Speed: %.3f MiB/s\n" "$duration" "$speed"
}

# Main loop for file sizes
for fs in "${FILE_SIZES_MB[@]}"; do
    test_file=$(create_test_file $fs)
    echo "Testing with file size: ${fs}MB"

    run_your_program "$test_file"
    run_dd "$test_file"

    # Optionally remove the test files after testing
    rm "$test_file"
    rm "fileB_${test_file}"
    rm "fileB_dd_${test_file}"
done
