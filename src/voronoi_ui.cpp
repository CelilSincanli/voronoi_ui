#include "voronoi_ui.hpp"
#include "stb_image.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Callback for GLFW errors
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

VoronoiUI::VoronoiUI() : window(nullptr), mainFont(nullptr), headingFont(nullptr), iconFont(nullptr), currentScreen(MAIN_SCREEN)  {}

VoronoiUI::~VoronoiUI() {
    Cleanup();
}

bool VoronoiUI::Initialize() {
    // Initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Set OpenGL version (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    window = glfwCreateWindow(1280, 720, "Voronoi Diagram Playground", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set the application icon
    // SetWindowIcon("include/assets/images/voronoi_transparent.png");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Set Dear ImGui style
    ImGui::StyleColorsDark();

    ImPlot::CreateContext(); 
    CustomizeImPlotInputMap();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // Load fonts
    ImFont* mainfont = io.Fonts->AddFontFromFileTTF(
        ASSET_PATH "/nasalization/nasalization rg.ttf", 18.5f, NULL, io.Fonts->GetGlyphRangesDefault());
     
    headingFont = io.Fonts->AddFontFromFileTTF(
        ASSET_PATH "/nasalization/nasalization rg.ttf", 30.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    
    ImFontConfig fontConfig;
    fontConfig.PixelSnapH = true;
    fontConfig.FontDataOwnedByAtlas = false;
    static const ImWchar iconRanges[] = { 0xe000, 0xf8ff, 0 };
    iconFont = io.Fonts->AddFontFromMemoryCompressedTTF(dripiconfont_compressed_data, dripiconfont_compressed_size, 28.0f, &fontConfig, iconRanges);
    
    static const ImWchar iconRanges2[] = { 0x20, 0x7F, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = false;
    icons_config.PixelSnapH = true;
    iconFont2 = io.Fonts->AddFontFromMemoryCompressedTTF(dripiconfont_compressed_data, dripiconfont_compressed_size, 28.0f, &icons_config, iconRanges2);

    return true;
}

void VoronoiUI::SetWindowIcon(const std::string& iconPath) {
    int width, height, channels;
    unsigned char* pixels = stbi_load(iconPath.c_str(), &width, &height, &channels, 4);
    if (pixels) {
        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = pixels;
        glfwSetWindowIcon(window, 1, &image);
        stbi_image_free(pixels);
    } else {
        std::cerr << "Failed to load icon: " << iconPath << std::endl;
    }
}

void VoronoiUI::Run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderUI();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void VoronoiUI::RenderUI() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Voronoi Diagram Playground", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (currentScreen == MAIN_SCREEN) {
        RenderMainScreen();
    } else if (currentScreen == NEW_DIAGRAM_SCREEN) {
        RenderNewDiagramScreen();
    }

    ImGui::End();
}

void VoronoiUI::RenderMainScreen() {

    ImVec2 window_size = ImGui::GetContentRegionAvail();
    ImGui::PushFont(headingFont);
    ImVec2 text_size = ImGui::CalcTextSize("Voronoi Diagram Playground");
    ImGui::PopFont();
    ImGui::PushFont(iconFont);
    ImVec2 icon_size = ImGui::CalcTextSize(u8"\uE016");
    ImGui::PopFont();

    float total_text_width = text_size.x + icon_size.x + 5.0f;
    float button_width = 200.0f;
    float button_height = 40.0f;
    float spacing = 20.0f;

    float total_height = text_size.y + spacing + button_height + spacing + button_height;
    float start_y = (window_size.y - total_height) * 0.5f;
    float text_x = (window_size.x - total_text_width) * 0.5f;

    ImGui::SetCursorPos(ImVec2(text_x, start_y));
    ImGui::PushFont(headingFont);
    ImGui::Text("Voronoi Diagram Playground");
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::PushFont(iconFont);
    ImGui::Text("%s", u8"\uE016");
    ImGui::PopFont();

    start_y += text_size.y + spacing;

    float button_x = (window_size.x - button_width) * 0.5f;
    
    ImGui::SetCursorPos(ImVec2(button_x, start_y));
    ImGui::PushFont(iconFont);
    ImGui::Text("%s", u8"\uE03F");
    ImGui::PopFont();
    ImGui::SameLine();
    
    if (ImGui::Button("Create Diagram", ImVec2(button_width, button_height))) {
        currentScreen = NEW_DIAGRAM_SCREEN;
    }

    start_y += button_height + spacing;

    ImGui::SetCursorPos(ImVec2(button_x, start_y));
    ImGui::PushFont(iconFont);
    ImGui::Text("%s", u8"\uE055"); 
    ImGui::PopFont();
    ImGui::SameLine();

    if (ImGui::Button("Load Diagram", ImVec2(button_width, button_height))) {
        std::cout << "Load Diagram button clicked!" << std::endl;
    }
}

void VoronoiUI::RenderNewDiagramScreen() {
    ImGui::Spacing();

    ImGuiIO& io = ImGui::GetIO();
    float screenWidth = io.DisplaySize.x;
    float screenHeight = io.DisplaySize.y;

    float frameWidth1 = screenWidth * 0.99f;
    float frameHeight1 = screenHeight * 0.06f;

    float frameWidth2 = screenWidth * 0.72f;
    float frameHeight2 = screenHeight * 0.88f;

    float frameWidth3 = screenWidth * 0.25f;
    float frameHeight3 = screenHeight * 0.88f;

    ImGui::BeginChild("FrameArea", ImVec2(frameWidth1, frameHeight1), false, ImGuiWindowFlags_NoScrollbar);
    {
        ImGui::PushFont(iconFont2);
        if (ImGui::Button("U", ImVec2(50.0f, 35.0f))) {
            currentScreen = MAIN_SCREEN;
        }
        ImGui::PopFont();
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2((screenWidth - frameWidth2) * 0.015f, frameHeight1 + 20.0f)); // Centered horizontally, below the first frame
    ImGui::BeginChild("CoordinateFrame", ImVec2(frameWidth2, frameHeight2), false, ImGuiWindowFlags_NoScrollbar);
    {
        static float x_data[1000];
        static float y_data[1000];
        static int point_count = 0;

        ImVec2 availableSize = ImGui::GetContentRegionAvail();

      if (ImPlot::BeginPlot("Voronoi Space", availableSize,  ImPlotFlags_NoLegend | ImPlotFlags_NoFrame | ImPlotFlags_NoMenus)){
            ImPlot::SetupAxes("X-Axis", "Y-Axis");
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, 10);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 5);

            if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0)) {
                ImPlotPoint mousePos = ImPlot::GetPlotMousePos();
                if (voronoi_points.size() < 1000) {
                    std::cout << "Mouse Position: (" << mousePos.x << ", " << mousePos.y << ")\n";
                    voronoi_points.push_back(Point_2(static_cast<double>(mousePos.x), static_cast<double>(mousePos.y)));
                    std::cout << "Last added point: (" << voronoi_points.back().x() << ", " << voronoi_points.back().y() << ")\n";
                
                    plotData.x_data[plotData.point_count] = mousePos.x;
                    plotData.y_data[plotData.point_count] = mousePos.y;
                    plotData.point_count++;
                }else{
                    ShowNotifications("Error", "Maximum number of points reached (1000).", 3000);
                }
            }

            if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(1)) { // Right mouse button
                if (voronoi_points.size() > 0) {
                    voronoi_points.pop_back();
                    plotData.point_count--; 
                    plotData.x_data[plotData.point_count] = 0.0f; 
                    plotData.y_data[plotData.point_count] = 0.0f; 
                }
            }
            if (voronoi_points.size() > 0) {
                ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 5.0f, ImVec4(255.0f / 255.0f, 125.0f / 255.0f, 125.0f / 255.0f, 1.0f));
                ImPlot::PlotScatter("Voronoi Points", plotData.x_data.data(), plotData.y_data.data(), plotData.point_count);
            }

            if (ImPlot::IsPlotHovered() && ImGui::IsMouseDown(2)) { // Middle mouse button
                 // Implement custom panning logic here if needed
                 // For example, adjust axis limits based on mouse movement
            }

            ImPlot::EndPlot();
        }
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(screenWidth - frameWidth3 - 10.0f, frameHeight1 + 20.0f)); // Positioned on the right, below the first frame
    ImGui::BeginChild("RightFrame", ImVec2(frameWidth3, frameHeight3), false, ImGuiWindowFlags_NoScrollbar);
    {
        RenderNotification();
        
        const char* drawButtonText = "Draw";
        ImVec2 textSize = ImGui::CalcTextSize(drawButtonText);

        float buttonWidth = textSize.x * 4.0f;
        float buttonHeight = textSize.y * 2.0f;

        float buttonX = (frameWidth3 - buttonWidth) * 0.5f;
        float buttonY = 20.0f;
        ImGui::SetCursorPos(ImVec2(buttonX, buttonY));
        if (ImGui::Button(drawButtonText, ImVec2(buttonWidth, buttonHeight))) {
            
            if (voronoi_points.size() < 1) {
                ShowNotifications("Error", "Please add at least one point to the diagram.", 3000);
            }
            else {
                UpdateVoronoiFaces(voronoi_points, voronoi_face_vertex_map);
                
                std::cout << "Voronoi Faces and their Vertices:\n";

                for (const auto& [site, vertices] : voronoi_face_vertex_map) {
                    // Print the site point of the face
                    std::cout << "Face for Site Point (" << site.x() << ", " << site.y() << "):\n";

                    // Print all vertices of the face
                    for (const auto& vertex : vertices) {
                        std::cout << "\tVertex: (" << vertex.x() << ", " << vertex.y() << ")\n";
                    }

                    // Add a separator for clarity
                    std::cout << "-----------------------------------\n";
                }
            }

        }

        const char* saveButtonText = "Save Diagram";
        buttonY += buttonHeight + 10.0f;
        ImGui::SetCursorPos(ImVec2(buttonX, buttonY));
        if (ImGui::Button(saveButtonText, ImVec2(buttonWidth, buttonHeight))) {
            if (voronoi_points.size() < 1) {
                ShowNotifications("Error", "Please add at least one point to the diagram.", 3000);
            }

        }

        const char* alignCenterText = "Align Center";
        textSize = ImGui::CalcTextSize(alignCenterText);

        buttonX = (frameWidth3 - buttonWidth) * 0.5f;
        buttonY = frameHeight3 - buttonHeight - 20.0f;
        ImGui::SetCursorPos(ImVec2(buttonX, buttonY));
        if (ImGui::Button(alignCenterText, ImVec2(buttonWidth, buttonHeight))) {
            if (voronoi_points.size() < 1) {
                ShowNotifications("Error", "Please add at least one point to the diagram.", 3000);
            }
        }
    }
    ImGui::EndChild();
}

void VoronoiUI::CustomizeImPlotInputMap() {
    ImPlotInputMap& inputMap = ImPlot::GetInputMap();

    inputMap.Pan = ImGuiMouseButton_Middle;
    inputMap.PanMod = 0;

    inputMap.Select = ImGuiMouseButton_Right;
    inputMap.SelectMod = 0;

}

void VoronoiUI::ShowNotifications(const std::string& title, const std::string& text, float duration_ms) {
    Notification notification;
    notification.title = title;
    notification.text = text;
    notification.duration = duration_ms / 1000.0f;
    notification.start_time = std::chrono::steady_clock::now();
    notifications.push_back(notification);
}

void VoronoiUI::RenderNotification() {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 250, ImGui::GetIO().DisplaySize.y - 100), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, 80), ImGuiCond_Always);

    for (int notification_index = 0; notification_index < notifications.size();)
    {
        Notification& notification = notifications[notification_index];
        float elapsed_time = std::chrono::duration<float>(std::chrono::steady_clock::now() - notification.start_time).count();

        if(elapsed_time > notification.duration) {
            notifications.erase(notifications.begin() + notification_index);
        }else{
            ImGui::Begin("Notification", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::Text("%s", notification.title.c_str());
            ImGui::Separator();
            ImGui::TextWrapped("%s", notification.text.c_str());
            ImGui::End();
            notification_index++;
        }
    }
    
}

void VoronoiUI::Cleanup() {
    if (window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    ImPlot::DestroyContext();
}