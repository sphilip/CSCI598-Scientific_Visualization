project(volume_rendering_unit)

# some needed gtest files
set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/../test_framework/gtest-1.6.0")
set(GTEST_SRC "${GTEST_ROOT}/src")
set(GTEST_INC "${GTEST_ROOT}/include")

enable_testing()
#include_directories(${GTEST_SRC} ${GTEST_INC})
include_directories(${GTEST_INC})

add_executable(run.vector_test src_vector.cc)
target_link_libraries(run.vector_test gtest gtest_main)

add_test(NAME run.vector_test COMMAND volume_rendering_unit)
