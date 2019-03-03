Just a simple bruteforce algorithm for fun :) 

1) First download dependencies clang or g++, cmake and libssl.

    on ubuntu/debian: sudo apt-get install cmake g++ clang-7 lldb-7 lld-7 libssl-dev 

2) export CC/CXX for cmake

    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++

    or  

    export CC=/usr/bin/gcc
    export CXX=/usr/bin/g++

2) run cmake 

cmake .

3) run make

TODO:

- [ ] rainbow tables
- [ ] multithreads
- [ ] CUDA
- [ ] improve options
- [ ] clean and improve understanding of the code
- [ ] compile with clang and g++, compare perf