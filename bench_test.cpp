#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>

void createTestFile(const char* filename, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    for (size_t i = 0; i < size; ++i) {
        file.put(static_cast<char>(rand() % 256));
    }
}

void copyFile(const char *inputFilePath, const char *outputFilePath, unsigned int blockSize) {
    int inputFd = open(inputFilePath, O_RDONLY);
    if (inputFd < 0) {
        perror("Error opening input file");
        return;
    }

    int outputFd = open(outputFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outputFd < 0) {
        perror("Error opening output file");
        close(inputFd);
        return;
    }

    char *buffer = (char *)malloc(blockSize);
    if (!buffer) {
        perror("Failed to allocate memory");
        close(inputFd);
        close(outputFd);
        return;
    }

    ssize_t bytesRead, bytesWritten;
    while ((bytesRead = read(inputFd, buffer, blockSize)) > 0) {
        bytesWritten = write(outputFd, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            perror("Error writing to output file");
            break;
        }
    }

    close(inputFd);
    close(outputFd);
    free(buffer);
}


// Google Benchmark function
static void BM_FileCopy(benchmark::State& state) {
    unsigned int blockSize = state.range(0);
    unsigned int fileSizeMB = state.range(1);
    std::string inputFile = "input_" + std::to_string(fileSizeMB) + "MB.dat"; 
    std::string outputFile = "output.dat";

    // Create a test file if it doesn't exist
    struct stat buffer;   
    if (stat(inputFile.c_str(), &buffer) != 0) {
        std::cout << "Creating a test file of size " << fileSizeMB << " MB\n";
        createTestFile(inputFile.c_str(), 1024 * 1024 * fileSizeMB); // Create a file of fileSizeMB MB
    }

    for (auto _ : state) {
        state.PauseTiming(); // Pause timing while setting up for the copy
        unlink(outputFile.c_str()); // Remove output file if it exists
        state.ResumeTiming(); // Resume timing for the copy operation

        copyFile(inputFile.c_str(), outputFile.c_str(), blockSize);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(blockSize));
}

// Register the function as a benchmark with a range of block sizes and file sizes
BENCHMARK(BM_FileCopy)->Args({4096, 1})->Args({4096, 10})->Args({4096, 100})->Args({4096, 1000});

BENCHMARK_MAIN();
