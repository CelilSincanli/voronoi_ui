cmake_minimum_required(VERSION 3.10)
project(voronoi_ui)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_definitions(-DASSET_PATH="${CMAKE_SOURCE_DIR}/include/assets/fonts")

add_executable(voronoi_ui 
    src/main.cpp
    src/voronoi_ui.cpp
    include/third_parties/imgui/imgui.cpp
    include/third_parties/imgui/imgui_draw.cpp
    include/third_parties/imgui/imgui_tables.cpp
    include/third_parties/imgui/imgui_widgets.cpp
    include/third_parties/imgui/backends/imgui_impl_glfw.cpp
    include/third_parties/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(voronoi_ui PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/third_parties/imgui 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/third_parties/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assets/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assets/images
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assets/icons
    ${CMAKE_CURRENT_SOURCE_DIR}/include/third_parties/stb
)

target_link_libraries(voronoi_ui glfw OpenGL::GL dl z)
