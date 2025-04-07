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

    // Initialize platform/renderer bindings
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
    static const ImWchar iconRanges[] = { 0xe000, 0xf8ff, 0 }; // Dripicons range
    iconFont = io.Fonts->AddFontFromMemoryCompressedTTF(dripiconfont_compressed_data, dripiconfont_compressed_size, 28.0f, &fontConfig, iconRanges);
    static const ImWchar iconRanges2[] = { 0x20, 0x7F, 0 }; // Dripicons range
    ImFontConfig icons_config;
    icons_config.MergeMode = false;
    icons_config.PixelSnapH = true;
    iconFont2 = io.Fonts->AddFontFromMemoryCompressedTTF(dripiconfont_compressed_data, dripiconfont_compressed_size, 28.0f, &icons_config, iconRanges2);

    return true;
}

void VoronoiUI::SetWindowIcon(const std::string& iconPath) {
    int width, height, channels;
    unsigned char* pixels = stbi_load(iconPath.c_str(), &width, &height, &channels, 4); // Load image as RGBA
    if (pixels) {
        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = pixels;
        glfwSetWindowIcon(window, 1, &image); // Set the icon
        stbi_image_free(pixels); // Free the image memory
    } else {
        std::cerr << "Failed to load icon: " << iconPath << std::endl;
    }
}

void VoronoiUI::Run() {
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render the UI
        RenderUI();

        // Render
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
    // Fullscreen ImGui window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Voronoi Diagram Playground", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (currentScreen == MAIN_SCREEN) {
        // Render the main screen
        RenderMainScreen();
    } else if (currentScreen == NEW_DIAGRAM_SCREEN) {
        // Render the new diagram screen
        RenderNewDiagramScreen();
    }

    ImGui::End();
}

void VoronoiUI::RenderMainScreen() {
    // Center the text and buttons
    ImVec2 window_size = ImGui::GetContentRegionAvail();
    ImGui::PushFont(headingFont);
    ImVec2 text_size = ImGui::CalcTextSize("Voronoi Diagram Playground");
    ImGui::PopFont();
    ImGui::PushFont(iconFont);
    ImVec2 icon_size = ImGui::CalcTextSize(u8"\uE016");
    ImGui::PopFont();

    float total_text_width = text_size.x + icon_size.x + 5.0f; // Add some padding between text and icon
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
        std::cout << "Create Diagram button clicked!" << std::endl;
        currentScreen = NEW_DIAGRAM_SCREEN; // Switch to the new diagram screen
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

    ImGui::PushFont(iconFont2);
    if (ImGui::Button("U", ImVec2(50.0f, 35.0f))) {
        currentScreen = MAIN_SCREEN;
    }
    ImGui::PopFont();
}

void VoronoiUI::Cleanup() {
    if (window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}