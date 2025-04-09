#ifndef VORONOI_UI_HPP
#define VORONOI_UI_HPP

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

// Icon implementation
#include "dripicon_v2.h"

class VoronoiUI {
public:
    VoronoiUI();
    ~VoronoiUI();

    bool Initialize();
    void Run();
    void Cleanup();

private:
    GLFWwindow* window;
    ImFont* mainFont;
    ImFont* headingFont;
    ImFont* iconFont;
    ImFont* iconFont2;
    
    void RenderMainScreen();
    void RenderNewDiagramScreen();
    enum Screen { MAIN_SCREEN, NEW_DIAGRAM_SCREEN };
    Screen currentScreen;

    void SetWindowIcon(const std::string& iconPath);
    void RenderUI();
};

#endif // VORONOI_UI_HPP