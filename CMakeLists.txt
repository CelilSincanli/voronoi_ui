cmake_minimum_required(VERSION 3.10)
project(voronoi_ui)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_definitions(-DASSET_PATH="${CMAKE_SOURCE_DIR}/include/assets/fonts")

add_executable(voronoi_ui 
    src/main.cpp
    include/third_parties/imgui/imgui.cpp
    include/third_parties/imgui/imgui_draw.cpp
    include/third_parties/imgui/imgui_tables.cpp
    include/third_parties/imgui/imgui_widgets.cpp
    include/third_parties/imgui/backends/imgui_impl_glfw.cpp
    include/third_parties/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(voronoi_ui PRIVATE 
    include/third_parties/imgui 
    include/third_parties/imgui/backends
    include/assets/fonts
    include/assets/icons
    include/assets/images
    ${CMAKE_CURRENT_SOURCE_DIR}/include/third_parties/stb
)

target_link_libraries(voronoi_ui glfw OpenGL::GL dl z)
