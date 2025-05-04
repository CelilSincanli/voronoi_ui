cmake_minimum_required(VERSION 3.10)
project(voronoi_ui)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(CGAL REQUIRED)

add_definitions(-DASSET_PATH="${CMAKE_SOURCE_DIR}/include/assets/fonts")

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/${PROJECT_NAME}.cpp
    src/voronoi.cpp
    include/third_parties/imgui/imgui.cpp
    include/third_parties/imgui/imgui_draw.cpp
    include/third_parties/imgui/imgui_tables.cpp
    include/third_parties/imgui/imgui_widgets.cpp
    include/third_parties/imgui/backends/imgui_impl_glfw.cpp
    include/third_parties/imgui/backends/imgui_impl_opengl3.cpp
    include/third_parties/implot/implot.cpp
    include/third_parties/implot/implot_items.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/third_parties/imgui 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/third_parties/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/include/third_parties/implot
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assets/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assets/images
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assets/icons
    ${CMAKE_CURRENT_SOURCE_DIR}/include/third_parties/stb
)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL CGAL::CGAL dl z)

add_custom_target(valgrind
    COMMAND valgrind --leak-check=full --show-leak-kinds=all ./${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
)