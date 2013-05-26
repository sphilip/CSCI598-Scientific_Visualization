project(volume_rendering_unit)

# some needed gtest files
set(GTEST_BUILD "${CMAKE_SOURCE_DIR}/../test_framework/build/gtest-1.6.0")
set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/../test_framework/gtest-1.6.0")
set(GTEST_SRC "${GTEST_ROOT}/src")
set(GTEST_INC "${GTEST_ROOT}/include")
file(GLOB GTEST_LIB ${GTEST_BUILD}/*.a)

set(IMAGE_LIB "${CMAKE_BINARY_DIR}/src/libimage.a")
set(VECTOR_LIB "${CMAKE_BINARY_DIR}/src/libvector.a")

message(STATUS "Linking tests")
enable_testing()
include_directories(${GTEST_INC})
add_executable(vector_test src_vector.cc)
target_link_libraries(vector_test ${GTEST_LIB} ${VECTOR_LIB})

message(STATUS "Creating custom test")
add_test(NAME vector_test COMMAND ${PROJECT_NAME})
#add_dependencies(vector_test ${VECTOR_LIB})

add_custom_target(
    run.vector_test ALL 
    COMMAND ${CMAKE_BINARY_DIR}/unit/vector_test
    DEPENDS vector_test)
