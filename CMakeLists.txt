cmake_minimum_required(VERSION 3.1)
project(example)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_OPENGL_GLFW_IMGUI "Use ImGui"          ON)
option(LIBIGL_WITH_CGAL "Use CGAL"          ON)
find_package(LIBIGL REQUIRED QUIET)
set(IMGUIZMO_PATH ${LIBIGL_SOURCE_DIR}/igl/opengl/glfw/imgui/imguizmo)

# Add the files containing your project's core functionality (but not exectuable
# main() functions)
file(GLOB SRCFILES include/*.cpp)
add_library(core ${SRCFILES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
# these files may depend on libigl
target_link_libraries(core igl::core igl::opengl igl::cgal)

# 
add_executable(swept-volumes-gui swept-volumes-gui.cpp)
set_target_properties (swept-volumes-gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/)
target_include_directories(swept-volumes-gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(swept-volumes-gui core igl::core igl::opengl_glfw igl::opengl_glfw_imgui)


