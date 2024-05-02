# Optimized-File-Management-System-Performance-Analysis

This project presents an Optimized File Management System for Performance Analysis. It focuses on benchmarking file I/O operations using custom-built programs and comparing these against the standard Unix dd command to analyze performance under various conditions.

## Getting Started

Follow these instructions to get the project running on your local machine for development and testing purposes.

### Prerequisites

- A Linux-based operating system.
- GCC Compiler
- Bash shell.
- Google Benchmark library for parts requiring benchmark testing.

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/Optimized-File-Management-System-Performance-Analysis.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Optimized-File-Management-System-Performance-Analysis
   ```

### Compiling the Project

To compile all necessary files, perform the following steps:

1. Make the build script executable:
   ```bash
   chmod +x build.sh
   ```
2. Execute the build script:
   ```bash
   ./build.sh
   ```

### Usage Instructions

Run the programs using the following commands based on the functionality you need to test:

- **Basic Operations:**
  ```bash
  ./run <filename> [-r|-w] <block_size> <block_count>
  ```

- **Performance Measurement:**
  ```bash
  ./run2 <filename> <block_size>
  ```

- **DD vs. Our Program Performance Test:**
  ```bash
  ./ddtest.sh
  ```

- **Google Benchmark Integration Test:**
  ```bash
  ./bench_test
  ```

- **Raw Performance Test:**
  ```bash
  ./run3 <filename> <block_size>
  ```

- **Cache Test:**
  ```bash
  sudo ./cachetest.sh <filename>
  ```

- **System Call Overhead Test:**
  ```bash
  ./syscall <filename> <block_size>
  ```

- **Advanced Performance Test:**
  ```bash
  ./fast <filename>
  ```
