#ifndef VORONOI_UI_HPP
#define VORONOI_UI_HPP

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <array>

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
    
    struct PlotData {
        std::array<float, 1000> x_data; // X-axis data
        std::array<float, 1000> y_data; // Y-axis data
        int point_count = 0;            // Number of points
    };

    PlotData plotData;
    
    void RenderMainScreen();
    void RenderNewDiagramScreen();
    void CustomizeImPlotInputMap();
    enum Screen { MAIN_SCREEN, NEW_DIAGRAM_SCREEN };
    Screen currentScreen;

    void SetWindowIcon(const std::string& iconPath);
    void RenderUI();
};

#endif // VORONOI_UI_HPP