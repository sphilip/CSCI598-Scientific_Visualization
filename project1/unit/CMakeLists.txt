project(volume_rendering_unit)

include(ExternalProject)

# some needed gtest files
set(GTEST_BUILD "${CMAKE_SOURCE_DIR}/../test_framework/build/gtest-1.6.0")
set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/../test_framework/gtest-1.6.0")
set(GTEST_SRC "${GTEST_ROOT}/src")
set(GTEST_INC "${GTEST_ROOT}/include")
file(GLOB GTEST_LIB ${GTEST_BUILD}/*.a)

#ExternalProject_Add(
#    googletest
#    SOURCE_DIR ${GTEST_ROOT}
#    BUILD_DIR ${CMAKE_BINARY_DIR}
#)

    
set(IMAGE_LIB "${CMAKE_BINARY_DIR}/src/libimage.a")
set(VECTOR_LIB "${CMAKE_BINARY_DIR}/src/libvector.a")

enable_testing()
#include_directories(${GTEST_SRC} ${GTEST_INC})
include_directories(${GTEST_INC})
message(status ${GTEST_LIB})
add_executable(vector_test src_vector.cc)
target_link_libraries(vector_test ${GTEST_LIB} ${VECTOR_LIB})

add_test(NAME vector_test COMMAND volume_rendering_unit)
