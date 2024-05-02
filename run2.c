#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

// Function to get the current time in seconds
double getCurrentTime() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Function to process the file and count the number of bytes and blocks read
void processFile(int fileDescriptor, int blockSize, unsigned long *byte_count, unsigned long *block_count) {
  ssize_t bytesRead;
  int bufferSize = blockSize / 4;
  int *buffer = malloc(bufferSize * sizeof(int));
  *byte_count = 0;
  *block_count = 0;

  // Read the file in blocks and update the byte and block count
  while ((bytesRead = read(fileDescriptor, buffer, blockSize)) > 0) {
    (*block_count)++;
    *byte_count += bytesRead;
  }

  // Check for errors during file read
  if(bytesRead < 0){
    printf("Error during file read\n");
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  int fileDescriptor;

  // Check for correct number of arguments
  if(argc != 3){
    printf("Error: Incorrect arguments. Usage: <filename> <block_size>\n");
    exit(0);
  }

  char *filename = argv[1];
  int blockSize = atoi(argv[2]);

  // Open the file and check for errors
  if((fileDescriptor = open(filename, O_RDONLY, 0666)) < 0){
    printf("Error: Cannot open %s\n", filename);
    exit(0);
  }
  
  // Measure the time taken to read the file
  double startTime = getCurrentTime();
  unsigned long byte_count, block_count;
  processFile(fileDescriptor, blockSize, &byte_count, &block_count);
  double endTime = getCurrentTime();

  // Print the results
  printf("Completed reading in %f seconds\n", endTime - startTime); 
  printf("Reading speed: %f MiB/s\n", (byte_count/ (double)(1024*1024)) / (endTime - startTime)); 
  printf("Total blocks read: %ld\n", block_count);
  printf("File size: %llu bytes\n", (unsigned long long)block_count * blockSize);

  // Close the file
  close(fileDescriptor);

  return 0;
}
