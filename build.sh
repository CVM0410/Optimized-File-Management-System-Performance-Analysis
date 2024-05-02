gcc run.c -o run
gcc run2.c -o run2
chmod +x ddtest.sh
gcc runtest.c -o runtest
g++ -std=c++11 bench_test.cpp -lbenchmark -lpthread -o bench_test
gcc run3.c -o run3
chmod +x cachetest.sh
gcc syscall.c -o syscall
gcc -Ofast -march=native -pipe -D_GNU_SOURCE -pthread fast.c -o fast
