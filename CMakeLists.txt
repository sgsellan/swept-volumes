cmake_minimum_required(VERSION 3.1)
project(example)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)

find_package(LIBIGL REQUIRED QUIET)

# Add the files containing your project's core functionality (but not exectuable
# main() functions)
file(GLOB SRCFILES include/*.cpp)
add_library(core ${SRCFILES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
# these files may depend on libigl
target_link_libraries(core igl::core)

# example1 requires libigl and opengl/glfw
add_executable(example1 example1.cpp)
target_include_directories(example1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(example1 core igl::core igl::opengl_glfw)

# example2 does not require opengl/glfw, so those are not linked here
add_executable(example2 example2.cpp)
target_include_directories(example2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(example2 core igl::core)
