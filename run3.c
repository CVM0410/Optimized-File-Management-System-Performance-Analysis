#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

// Function to get the current time in seconds.
double getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL); // Get the current time
    return tv.tv_sec + tv.tv_usec / 1000000.0; // Convert to seconds
}

// Function to compute the XOR of all elements in a buffer.
unsigned int computeXOR(unsigned int *buffer, unsigned int length)
{
    unsigned int result = 0; // Initialize XOR result
    for (unsigned int i = 0; i < length; ++i)
    {
        result ^= buffer[i]; // Compute XOR
    }
    return result;
}

// Function to read a file and calculate XOR of its contents.
unsigned int processFile(char *filePath, unsigned int blockSize)
{
    unsigned int bufferSize = blockSize / sizeof(unsigned int); // Calculate buffer size
    unsigned int buffer[bufferSize];
    unsigned int totalXOR = 0; // XOR of all blocks
    int fileDescriptor = open(filePath, O_RDONLY); // Open the file
    if (fileDescriptor < 0)
    {
        perror("Error opening file");
        return 0;
    }

    ssize_t bytesRead;
    unsigned int blockCount = 0;
    while ((bytesRead = read(fileDescriptor, buffer, blockSize)) > 0)
    {
        blockCount++;
        // Check if the last block is not a multiple of 4 bytes
        if (bytesRead < blockSize)
        {
            // Calculate how many bytes to pad
            unsigned int paddingBytes = blockSize - bytesRead;
            // Pad the remaining bytes of the buffer with 0x00
            unsigned char *byteBuffer = (unsigned char *)buffer;
            for (unsigned int i = bytesRead; i < blockSize; i++)
            {
                byteBuffer[i] = 0x00;
            }
        }
        totalXOR ^= computeXOR(buffer, bufferSize); // Update total XOR
    }
    printf("Total XOR: %08x\n", totalXOR);
    close(fileDescriptor);
    return blockCount;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <filename> <block_size>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    unsigned int blockSize = atoi(argv[2]);

    double startTime = getCurrentTime(); // Start time for reading the file
    unsigned int blocksProcessed = processFile(filename, blockSize); // Process the file
    double endTime = getCurrentTime(); // End time after reading the file

    double readDuration = endTime - startTime; // Calculate read duration
    unsigned long long totalBytesProcessed = (unsigned long long) blockSize * blocksProcessed;
    double readSpeed = (double) totalBytesProcessed / 1024.0 / 1024.0 / readDuration; // Calculate read speed in MiB/s

    printf("Reading Time: %.2f seconds\n", readDuration);
    printf("Read speed: %.2f MiB/s\n", readSpeed);

    return 0;
}
