project(volume_rendering_src)

# create static libs from other classes
add_library(RGB STATIC RGB.cpp)
add_library(Image STATIC image.cpp)
add_library(Vector STATIC Vector.cpp)

# need to link RGB symbols in Image to original lib
target_link_libraries(Image RGB)

# create executable from sources
add_executable(volume_renderer raytrace.cpp)

# need to link symbols from static libs
target_link_libraries(volume_renderer Image Vector)
