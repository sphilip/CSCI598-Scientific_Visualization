project(test_framework)
cmake_minimum_required(VERSION 2.8)

set(GTEST_VERSION "gtest-1.7.0")
# 1st extract the zip files
file(
  GLOB 
  ZIP_FILES 
  *.zip
)

message(STATUS  "Found zipped files: ${ZIP_FILES}")

# create separate directory for gtest
execute_process(COMMAND unzip ${ZIP_FILES}) 

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GTEST_ROOT})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GTEST_ROOT})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GTEST_ROOT}) 

# compile gtest
set_property(GLOBAL PROPERTY GTEST_ROOT_GLOBAL "${CMAKE_BINARY_DIR}/${GTEST_VERSION}")
set_property(GLOBAL PROPERTY GTEST_BUILD_GLOBAL "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
set(GTEST_ROOT "${CMAKE_BINARY_DIR}/${GTEST_VERSION}")
set(GTEST_ROOT_SRC "${GTEST_ROOT}/src")
set(GTEST_ROOT_INC "${GTEST_ROOT}/include")

message(status "${GTEST_ROOT}/cmake/internal_utils.cmake)") 
include(${GTEST_ROOT}/cmake/internal_utils.cmake)
set(GTEST_HAS_PTHREAD 0)
set(gtest_disable_pthreads ON)
config_compiler_and_linker()

include_directories(${GTEST_ROOT_INC} ${GTEST_ROOT})
link_directories(${GTEST_ROOT}/src)

cxx_library(gtest "${cxx_strict}" ${GTEST_ROOT_SRC}/gtest-all.cc)
cxx_library(gtest_main "${cxx_strict}" ${GTEST_ROOT_SRC}/gtest_main.cc)

target_link_libraries(gtest_main gtest)
