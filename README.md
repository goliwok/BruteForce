Just a simple bruteforce algorithm for fun :) 

	
## Examples

* Brute force a zip file from wordlist:

`cat examples/wordlist.txt | ./Bf  --mode stdin --crack zip --file ./encrypted.zip`

* Brute force a zip file from string permutations:

`./Bf  --mode random --crack zip --file ./encrypted.zip`

1) First download dependencies clang or g++, cmake, Opencl and libssl.

    on ubuntu/debian: sudo apt-get install cmake g++ clang-7 lldb-7 lld-7 libssl-dev 

    #############################################
    
    FOR OPENCL, I DON'T KNOW HOW TO INSTALL IT ON UBUNTU/DEBIAN SYSTEM, GOOGLE IS YOUR FRIEND :)
    
    #############################################

2) export CC/CXX for cmake

    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++

    or  

    export CC=/usr/bin/gcc
    export CXX=/usr/bin/g++

2) run cmake 

    cmake .

3) run make

    make

TODO:

- [ ] rainbow tables
- [ ] multithreads
- [ ] Graphic card (openCL/CUDA)
- [X] improve arguments
- [ ] clean and improve understanding of the code
- [ ] compile with clang and g++, compare perf
- [ ] Zip
- [ ] sha3
- [ ] HMAC
- [ ] AES 256
