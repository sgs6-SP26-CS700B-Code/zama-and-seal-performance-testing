.SILENT:
.PHONY: all test format clean clean_deep init init_submodules init_packages docs
.DEFAULT_GOAL := all
SHELL = /bin/bash

# We use 2 less threads to free up compute for IDE and OS.
# If your system has less than 2 threads, it will underflow to the max threads.
# If it has specifically 2 threads, it will currently cause an error. If this occurs modify the value below.
AVAILABLE_THREADS = $(shell echo $$(( $(shell nproc) - 2 )))

all:
	CC=$(CC) CXX=$(CXX) cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo ; \
	cmake --build build -j "$(AVAILABLE_THREADS)" ;


all_slow:
	CC=$(CC) CXX=$(CXX) cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo ; \
	cmake --build build -j "1" ;

clean:
	cmake --build build --target clean ;

clean_deep:
	rm -rf ./build/* ; \
	rm -rf ./.cache/* ;

format:
	bash ./scripts/formatCode.sh ;

init_submodules:
	git submodule update --init --recursive ;
