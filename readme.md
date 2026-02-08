
# SEAL  Zama Comparison

---

This repo is for testing SEAL and Zama for performance comparisons between the following operations:
- Addition
- Multiplication
- Batching (Both?)

It is planned to be tested against 64 bit signed numbers, but 32 bit numbers were prepared as well, and 128 bit numbers will be generated but may not be used.

This repo is part of a collection of testing / learning repos scrapped together for CS700B to gain an understanding of various algorithms and methods to do FHE and run it in SGX. Unlike the others, I decided to make this a more... simple project structure to make it easier to tell whats being compared rather than getting familiar with APIs. 

Additionally, the data used is included with this repo to keep things simple, even though it does realistically make the git repo much larger.

---

# Build Instructions

---

Preparation:
- Build on a Linux os with a GCC version supporting C++ 17 minimally.
- This project was tested on Ubuntu 24.04, but may work on other linux distros, we have not tested this
- Run `make init` to initialize the git submodules for SEAL and the TFHE C library
- There may be some extra dependencies since the TFHE C Library is written in rust and I have never compiled a rust app before

Building:
- `make all` will build the whole repo in the `./build/` folder.
- `make clean` will do basic cleaning in the build folder.
- `make clean_deep` will fully remove the contents of the build folder and the cache folder. 

Developing:
- `make format` will apply the .clang-format rules to the source code - requires `clang-format`
- The project uses clangd for its static analysis, and uses a clang-tidy file for additional best practice guiding
- If you want to use syntaxt highlighting, static analysis, etc on VSCode, I recommend disabling the C/C++ extensions intelisesnse and using the clangd extension as it provides much more accurate code analysis by using the compile commands file which tells the analyzer precisely how the program is compiled to know what is accessible where. 