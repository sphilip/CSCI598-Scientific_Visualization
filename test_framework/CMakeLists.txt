project(test_framework)
cmake_minimum_required(VERSION 2.8)

set(GTEST_BUILD "${CMAKE_BINARY_DIR}/gtest-1.6.0")
file(MAKE_DIRECTORY ${GTEST_BUILD})

set(GTEST_OUTPUT "")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GTEST_BUILD})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GTEST_BUILD})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GTEST_BUILD}) 

# compile gtest
set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/gtest-1.6.0")
set(GTEST_ROOT_SRC "${GTEST_ROOT}/src")
set(GTEST_ROOT_INC "${GTEST_ROOT}/include")

message(status "${GTEST_ROOT}/cmake/internal_utils.cmake)") 
include(${GTEST_ROOT}/cmake/internal_utils.cmake)
set(GTEST_HAS_PTHREAD 0)
set(gtest_disable_pthreads ON)
config_compiler_and_linker()

include_directories(${GTEST_ROOT_INC} ${GTEST_ROOT})
link_directories(${GTEST_BUILD}/src)

cxx_library(gtest "${cxx_strict}" ${GTEST_ROOT_SRC}/gtest-all.cc)
cxx_library(gtest_main "${cxx_strict}" ${GTEST_ROOT_SRC}/gtest_main.cc)

target_link_libraries(gtest_main gtest)