
# SEAL  Zama Comparison

---

This repo is for testing SEAL and Zama for performance comparisons between the following operations:
- Addition
- Multiplication
- Batching (Seal Only, ZAMA does not support batching fundamentally as it is based on Torus FHE, torus based cryptography is incompatible with the concept)

It is planned to be tested against 64 bit signed numbers, but 32 bit numbers were prepared as well, and 128 bit numbers will be generated but may not be used.

This repo is part of a collection of testing / learning repos scrapped together for CS700B to gain an understanding of various algorithms and methods to do FHE and run it in SGX. Unlike the others, I decided to make this a more... simple project structure to make it easier to tell whats being compared rather than getting familiar with APIs. 

Additionally, the data used is included with this repo to keep things simple, even though it does realistically make the git repo much larger.


---

# Current State

---

Current configuration is for zama and seal to run, seal first with all tests, zama multi threaded for a small amount of tests. I need to figure out the fine tuning for SEAL parameters (or see if there is an issue with overflows from signed values?), particularly for multiplication. That being said, the current code is whats being used to collect the next stage of data. The next batch will try to fix these.

Additionally, Packing for Zama was investigated partially, but as I looked deeper into how to test it and measure it, it turns out I would need two dedicated apps developed to test it so I can accurately compare memory usage of applications before and after. The code I was working on can be found in the src directory in .old files.

---

# Build Instructions

---

Preparation:
- Build on a Linux os with a GCC version supporting C++ 17 minimally.
- This project was tested on Ubuntu 24.04, but may work on other linux distros, we have not tested this
- Run `make init` to initialize the git submodules for SEAL and the TFHE C library
- Ensure cargo and rustup is installed (`sudo apt install cargo rustup`) for building rust apps (zama is rust with a C api)
- Install rust nightly build chain with `rustup install nightly`
- There may be some extra dependencies since the TFHE C Library is written in rust and I have never compiled a rust app before
- Compile the zama-rs C api with `make build_zama`

Preparing Zama:
The following build steps were tested and validated on a ubuntu 24.04 desktop os system. Not the second the last command actually builds the library, there are some warnings in it interestingly, but I suspect this may also tie in with it requiring the nightly release of rust or cargo or something? I am not familiar with rust and its build system beyond that similar to go-lang and python in some ways it hides dependencies frequently.
- `sudo apt install cargo rustup`
- `cd submodules/zama-rs/`
- `rustup install nightly`
- `rustup default nightly`
- `rustup target add x86_64-unknown-linux-gnu` - Note this appears to install a crap tun of random things, thank rust for being a shady build system and not asking if you want xyz packages as part of the package mentioned.
- `RUSTFLAGS="-C target-cpu=native" cargo +nightly build --release --features=high-level-c-api -p tfhe`
- `cd ../..`

Building:
- `make all` will build the whole repo in the `./build/` folder.
- `make clean` will do basic cleaning in the build folder.
- `make clean_deep` will fully remove the contents of the build folder and the cache folder. 

Developing:
- `make format` will apply the .clang-format rules to the source code - requires `clang-format`
- The project uses clangd for its static analysis, and uses a clang-tidy file for additional best practice guiding
- If you want to use syntaxt highlighting, static analysis, etc on VSCode, I recommend disabling the C/C++ extensions intelisesnse and using the clangd extension as it provides much more accurate code analysis by using the compile commands file which tells the analyzer precisely how the program is compiled to know what is accessible where. 