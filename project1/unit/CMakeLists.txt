project(volume_rendering_unit)

get_property(GTEST_BUILD
             GLOBAL PROPERTY GTEST_BUILD_GLOBAL)
get_property(GTEST_ROOT
             GLOBAL PROPERTY GTEST_ROOT_GLOBAL)

set(GTEST_SRC "${GTEST_ROOT}/src")
set(GTEST_INC "${GTEST_ROOT}/include")
file(GLOB GTEST_LIB ${GTEST_BUILD}/*.a)
message(STATUS "Found following gtest static libraries:${GTEST_LIB}")

get_property(PROJECT1_LIBS
             GLOBAL PROPERTY PROJECT1_LIBS_GLOBAL)
set(IMAGE_LIB "${PROJECT1_LIBS}/libimage.a")
set(VECTOR_LIB "${PROJECT1_LIBS}/libvector.a")

message(STATUS "*******Linking tests******")
enable_testing()
include_directories(${GTEST_INC})
add_executable(vector_test src_vector.cc)
target_link_libraries(vector_test ${GTEST_LIB} ${VECTOR_LIB})

message(STATUS "Creating custom test")
add_test(NAME vector_test COMMAND ${PROJECT_NAME})
add_dependencies(vector_test ${VECTOR_LIB})

add_custom_target(
    run.vector_test ALL 
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/vector_test
    DEPENDS vector_test)
