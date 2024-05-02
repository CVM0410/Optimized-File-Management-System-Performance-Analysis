#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdatomic.h>
#include <time.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

long int totalBlocks;
atomic_llong currentBlock = 0;

typedef struct {
    long int blocksize;
    unsigned int *mappedFile;
    unsigned int xorValue;
    long int fileLength;
} ThreadParameters;

// Function to calculate XOR of a block of data.
unsigned int calculateXOR(unsigned int *data, long int len) {
    unsigned int xorSum = 0;
    for (long int i = 0; i < len; i++) {
        xorSum ^= data[i];
    }
    return xorSum;
}

// Function executed by each thread to process a block of the file.
void *processBlock(void *parameters) {
    ThreadParameters *threadParams = (ThreadParameters *)parameters;
    long int blockStart;
    unsigned int xorThread = 0;
    while (1) {
        blockStart = atomic_fetch_add(&currentBlock, 1);
        if (blockStart * threadParams->blocksize >= threadParams->fileLength) {
            break;
        }
        long int offset = blockStart * threadParams->blocksize / sizeof(unsigned int);
        long int blockSize = threadParams->blocksize / sizeof(unsigned int);
        if (offset + blockSize > threadParams->fileLength / sizeof(unsigned int)) {
            blockSize = threadParams->fileLength / sizeof(unsigned int) - offset;
        }
        xorThread ^= calculateXOR(threadParams->mappedFile + offset, blockSize);
    }
    threadParams->xorValue = xorThread;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "Usage: %s <file_path> [number_of_threads] [block_size]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int numThreads;
    long int blockSize;

    // Set default values or use provided arguments
    const int defaultNumThreads = 8;
    const long int defaultBlockSize = 131072;

    if (argc == 2) {
        numThreads = defaultNumThreads;
        blockSize = defaultBlockSize;
    } else {
        numThreads = atoi(argv[2]);
        blockSize = (argc == 4) ? atol(argv[3]) : defaultBlockSize;
    }

    // Open file and check for errors
    int fileDesc = open(filename, O_RDONLY);
    if (fileDesc == -1) handle_error("Error opening file");

    // Get file size
    long int fileSize = lseek(fileDesc, 0, SEEK_END);
    if (fileSize == -1) handle_error("lseek failed");

    // Map file into memory
    unsigned int *mappedFile = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fileDesc, 0);
    if (mappedFile == MAP_FAILED) handle_error("Error mapping file");

    // Calculate total number of blocks
    totalBlocks = fileSize / blockSize + (fileSize % blockSize != 0);

    // Allocate threads and thread arguments
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    ThreadParameters *threadArgs = malloc(numThreads * sizeof(ThreadParameters));

    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create threads
    for (int i = 0; i < numThreads; i++) {
        threadArgs[i].blocksize = blockSize;
        threadArgs[i].mappedFile = mappedFile;
        threadArgs[i].fileLength = fileSize;
        threadArgs[i].xorValue = 0;
        if (pthread_create(&threads[i], NULL, processBlock, &threadArgs[i])) {
            handle_error("Error creating thread");
        }
    }

    // Join threads and accumulate XOR result
    unsigned int finalXOR = 0;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        finalXOR ^= threadArgs[i].xorValue;
    }

    // Calculate elapsed time
    clock_gettime(CLOCK_MONOTONIC, &finish);
    double timeElapsed = (finish.tv_sec - start.tv_sec);
    timeElapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    // Print results
    printf("Total XOR: %08x\n", finalXOR);
    printf("Time taken to read file: %.2f seconds\n", timeElapsed);
    printf("Reading speed: %.2f MiB/s\n", (fileSize / (1024.0 * 1024.0)) / timeElapsed);

    // Clean up
    free(threads);
    free(threadArgs);
    munmap(mappedFile, fileSize);
    close(fileDesc);
    return 0;
}
