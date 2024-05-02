#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

// Function to get the current time in seconds.
double getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Function to compute the XOR of all elements in an integer buffer.
unsigned int computeXOR(unsigned int *buffer, unsigned int length) {
    unsigned int result = 0;
    for (unsigned int i = 0; i < length; ++i) {
        result ^= buffer[i];  // XOR each element with the result.
    }
    return result;
}

// Function to calculate the XOR of the contents of a file.
unsigned int calculateFileXOR(char *filePath, unsigned int blockSize) {
    unsigned int bufferSize = blockSize / sizeof(unsigned int); // Calculate buffer size.
    unsigned int buffer[bufferSize];
    unsigned int totalXOR = 0; // Initialize XOR result.
    int fileDescriptor = open(filePath, O_RDONLY); // Open file for reading.
    if (fileDescriptor < 0) {
        perror("Error opening file");
        return 0;
    }

    unsigned int bytesRead;
    // Read the file block by block and compute XOR.
    while ((bytesRead = read(fileDescriptor, buffer, blockSize)) > 0) {
        totalXOR ^= computeXOR(buffer, bytesRead / sizeof(unsigned int));
    }
    close(fileDescriptor); // Close the file.
    return totalXOR;
}

// Function to write a file with a specified block size and count.
void processFileWrite(char *filePath, unsigned int blockSize, unsigned int blockCount) {
    char *buffer = (char *)malloc(blockSize);
    memset(buffer, 'f', blockSize); // Initialize buffer with 'f'.

    int fileDescriptor = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open file for writing.
    if (fileDescriptor < 0) {
        perror("Error opening file for write");
        free(buffer);
        return;
    }

    unsigned int totalBytesWritten = 0;
    for (unsigned int i = 0; i < blockCount; ++i) {
        ssize_t bytesWritten = write(fileDescriptor, buffer, blockSize); // Write to file.
        if (bytesWritten < 0) {
            perror("Error writing to file");
            break;
        }
        totalBytesWritten += bytesWritten;
    }
    printf("Total file size written: %u bytes\n", totalBytesWritten);

    close(fileDescriptor); // Close the file.
    free(buffer); // Free the buffer.
}

// Main function to handle input arguments and execute read or write operations.
int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <filename> [-r|-w] <block_size> <block_count>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char mode = argv[2][1];
    unsigned int blockSize = atoi(argv[3]);
    unsigned int blockCount = atoi(argv[4]);

    double startTime, endTime, operationDuration;
    unsigned int xorValue;

    startTime = getCurrentTime(); // Start timing the operation.
    if (mode == 'r' || mode == 'R') {
        xorValue = calculateFileXOR(filename, blockSize); // Perform read operation.
        printf("File XOR: %08x\n", xorValue);
    } else if (mode == 'w' || mode == 'W') {
        processFileWrite(filename, blockSize, blockCount); // Perform write operation.
    } else {
        fprintf(stderr, "Invalid mode. Please use -r for read or -w for write.\n");
        return 1;
    }
    endTime = getCurrentTime(); // End timing the operation.

    operationDuration = endTime - startTime;
    printf("Operation Time: %.2f seconds\n", operationDuration);

    return 0;
}
