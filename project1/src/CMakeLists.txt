project(volume_rendering_src)

# create static libs from other classes
set_property(GLOBAL PROPERTY 
             PROJECT1_LIBS_GLOBAL ${CMAKE_CURRENT_BINARY_DIR})
add_library(image STATIC image.cpp RGB.cpp)
add_library(vector STATIC Vector.cpp)

# create executable from sources
add_executable(volume_renderer raytrace.cpp)

# need to link symbols from static libs
target_link_libraries(volume_renderer image vector)
