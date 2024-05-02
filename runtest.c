#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

// Function to get the current time in seconds.
double getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL); // Gets the current time.
    return tv.tv_sec + tv.tv_usec / 1000000.0; // Converts time to seconds.
}

// Function to copy the contents of one file to another.
void copyFile(char *inputFilePath, char *outputFilePath, unsigned int blockSize) {
    // Open the input file for reading.
    int inputFd = open(inputFilePath, O_RDONLY);
    if (inputFd < 0) {
        perror("Error opening input file");
        return;
    }

    // Open the output file for writing (create if it doesn't exist, truncate if it does).
    int outputFd = open(outputFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outputFd < 0) {
        perror("Error opening output file");
        close(inputFd); // Close input file before returning.
        return;
    }

    // Allocate memory for the buffer.
    char *buffer = (char *)malloc(blockSize);
    if (!buffer) {
        perror("Failed to allocate memory");
        close(inputFd);
        close(outputFd);
        return;
    }

    // Read from the input file and write to the output file.
    ssize_t bytesRead;
    while ((bytesRead = read(inputFd, buffer, blockSize)) > 0) {
        write(outputFd, buffer, bytesRead);
    }

    // Close both files and free the buffer.
    close(inputFd);
    close(outputFd);
    free(buffer);
}

// Main function: parses arguments and calls the copy file function.
int main(int argc, char *argv[]) {
    // Check for the correct number of arguments.
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <output_file> <block_size>\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1]; // Input file path.
    char *outputFile = argv[2]; // Output file path.
    unsigned int blockSize = atoi(argv[3]); // Block size for copying.

    // Record start time of the operation.
    double startTime = getCurrentTime();
    copyFile(inputFile, outputFile, blockSize); // Perform the copy operation.
    // Record end time of the operation.
    double endTime = getCurrentTime();

    // Calculate and print the duration of the copy operation.
    double operationDuration = endTime - startTime;
    printf("Copy operation time: %.2f seconds\n", operationDuration);

    return 0;
}
