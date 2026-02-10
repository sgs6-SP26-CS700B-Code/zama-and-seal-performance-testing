file(GLOB PERFORMANCE_TEST_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB PERFORMANCE_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(EXECUTABLE_NAME seal_zama_performance_test)

add_executable(${EXECUTABLE_NAME} ${PERFORMANCE_TEST_HDRS} ${PERFORMANCE_TEST_SRC})

# Include current directory
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link against tfhe, and other necessary libraries
target_link_libraries(${EXECUTABLE_NAME} LINK_PUBLIC SEAL::seal tfhe m pthread dl)
add_dependencies(${EXECUTABLE_NAME} SEAL::seal)

# Ensure warnings are treated as errors
target_compile_options(${EXECUTABLE_NAME} PRIVATE -Werror)

# Ensure C++17 is used
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_17)
