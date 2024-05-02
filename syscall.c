#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/time.h>
#include<math.h>

// Function to get the current time
double getCurrentTime() 
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main(int argc, char*argv[]) 
{ 
    int file1, file2;
    char buffer[10000];
    double startTime, endTime;
    if(argc <= 1)
    {
        printf("Usage: ./syscall <filename>  <block_size>\n");
        return -1;
    }
    char *filename = argv[1];
    int blockSize = 1;

    // Open the file
    file1 = open(filename, O_RDWR, 0);
    if(file1<0)
    {
        printf("Error: The specified filename does not exist.\n");
    }

    // Measure the time taken for read operation
    {
    startTime = getCurrentTime();
    for(int y=0; y<(1024*1024);y++)
    {
        read(file1,buffer,blockSize);   
    }
    endTime = getCurrentTime();
    double timeTaken = endTime - startTime;
    double performance = 1/ timeTaken;
    double performance_B = performance*1024*1024;
    printf("\nFile size: %d bytes",1024*1024);
    printf("\nRead operation: Time taken = %f seconds",timeTaken);
    printf("\nRead operation: Performance at block size %d = %f MiB/s",blockSize,performance);
    printf("\nRead operation: Performance at block size %d = %f B/s\n",blockSize,performance_B);
    }

    // Measure the time taken for lseek operation
    {
    startTime = getCurrentTime();
    for(int y=0; y<(1024*1024);y++)
    {
        lseek (file1, 0, SEEK_CUR);   
    }
    endTime = getCurrentTime();
    double timeTaken = endTime - startTime;
    double performance = 1/ timeTaken;
    double performance_B = performance*1024*1024;
    printf("\nFile size: %d bytes",1024*1024);
    printf("\nlseek operation: Time taken = %f seconds",timeTaken);
    printf("\nlseek operation: Performance at block size %d = %f MiB/s",blockSize,performance);
    printf("\nlseek operation: Performance at block size %d = %f B/s\n",blockSize,performance_B);
    }

    // Measure the time taken for write operation
    file2 = open("test.txt", O_RDWR, 0);
    {
    startTime = getCurrentTime();
    for(int y=0; y<(1024*1024);y++)
    {
        write(file2,"1", 1);   
    }
    endTime = getCurrentTime();
    double timeTaken = endTime - startTime;
    double performance = 1/ timeTaken;
    double performance_B = performance*1024*1024;
    printf("\nFile size: %d bytes",1024*1024);
    printf("\nWrite operation: Time taken = %f seconds",timeTaken);
    printf("\nWrite operation: Performance at block size %d = %f MiB/s",blockSize,performance);
    printf("\nWrite operation: Performance at block size %d = %f B/s\n",blockSize,performance_B);
    }  

    // Measure the time taken for open operation
    {
    startTime = getCurrentTime();
    for(int y=0; y<(1024*1024);y++)
    {
        file2 = open("test.txt", O_RDWR, 0);   
    }
    endTime = getCurrentTime();
    double timeTaken = endTime - startTime;
    double performance = 1/ timeTaken;
    double performance_B = performance*1024*1024;
    printf("\nFile size: %d bytes",1024*1024);
    printf("\nOpen operation: Time taken = %f seconds",timeTaken);
    printf("\nOpen operation: Performance at block size %d = %f MiB/s",blockSize,performance);
    printf("\nOpen operation: Performance at block size %d = %f B/s\n",blockSize,performance_B);
    } 
    close(file2); 
} 
