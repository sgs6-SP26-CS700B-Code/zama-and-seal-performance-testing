cmake_minimum_required(VERSION 3.10)

# Project name
project(SEAL_EXPERIMENTS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build all libraries as static libraries" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/data)

# set(IPP_INCLUDE_DIR "/home/forbin/intel/oneapi/ippcp/latest/include/ippcp")
# set(IPP_LIB_DIR "/home/forbin/intel/oneapi/ippcp/latest/lib")

message("" )
message("-=-=-=-=-=-=-=-=-=-=-=-[Compiler Versions]-=-=-=-=-=-=-=-=-=-=-=-" )
message("CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message("Starting CXX Flags: ${CMAKE_CXX_FLAGS}")
message("Starting EXE Linker Flags: ${CMAKE_EXE_LINKER_FLAGS}")
message("CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" )
message("" )


if(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "DebugWithSanitizer")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "UnitTests")
  include(CTest)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "UnitTestsWithSanitizer")
  include(CTest)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()


message("" )
message("-=-=-=-=-=-=-=-=-=-=-=-[Build Configuration]-=-=-=-=-=-=-=-=-=-=-" )
message("CMAKE_CXX_COMPILER: " ${CMAKE_BUILD_TYPE} )
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_FLAGS}")
message("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" )
message("" )


# Adds external Libs manually
add_subdirectory(./submodules/seal/)


set(TFHE_C_API "submodules/zama-rs/target/release/")
include_directories(${TFHE_C_API})
include_directories(${TFHE_C_API}/deps)
add_library(tfhe STATIC IMPORTED)
set_target_properties(tfhe PROPERTIES IMPORTED_LOCATION ${TFHE_C_API}/libtfhe.a)


# Add the internal src libraries for the executable
add_subdirectory(src)

# Define source and destination paths
set(CONFIG_FILES_DIR "${CMAKE_SOURCE_DIR}/data")
set(BUILD_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

# Copy each configuration file from config/ to build/bin/
file(GLOB CONFIG_FILES "${CONFIG_FILES_DIR}/*")
foreach(CONFIG_FILE ${CONFIG_FILES})
    configure_file(${CONFIG_FILE} ${BUILD_BIN_DIR} COPYONLY)
endforeach()
