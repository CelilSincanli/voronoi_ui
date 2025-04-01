#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Icon implementation
#include "dripicon_v2.h"

//Loading image files
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Callback for GLFW errors
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Function to set the application icon
void SetWindowIcon(GLFWwindow* window, const char* icon_path) {
    int width, height, channels;
    unsigned char* pixels = stbi_load(icon_path, &width, &height, &channels, 4); // Load image as RGBA
    if (pixels) {
        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = pixels;
        glfwSetWindowIcon(window, 1, &image); // Set the icon
        stbi_image_free(pixels); // Free the image memory
    } else {
        std::cerr << "Failed to load icon: " << icon_path << std::endl;
    }
}

int main() {
    // Initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Voronoi Diagram Playground", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the application icon
    SetWindowIcon(window, "include/assets/icons/voronoi_transparent.png");


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

    ImFont* mainfont = io.Fonts->AddFontFromFileTTF(
        ASSET_PATH "/nasalization/nasalization rg.ttf", 18.5f, NULL, io.Fonts->GetGlyphRangesDefault());
    
    ImFont* mainfont_heading = io.Fonts->AddFontFromFileTTF(
        ASSET_PATH "/nasalization/nasalization rg.ttf", 30.0f, NULL, io.Fonts->GetGlyphRangesDefault());

    ImFontConfig font2_config;
    font2_config.PixelSnapH = true; // Sharper font
    font2_config.FontDataOwnedByAtlas = false; // To able to manage meemry by myself
    static const ImWchar icon_ranges[] = { 0xe000, 0xf8ff, 0 };  // Dripicons range
    ImFont* iconfont = io.Fonts->AddFontFromMemoryCompressedTTF(dripiconfont_compressed_data, dripiconfont_compressed_size, 28.0f, &font2_config, icon_ranges);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Fullscreen ImGui window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("Voronoi Diagram Playground", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // Center the text and buttons
        ImVec2 window_size = ImGui::GetContentRegionAvail();
        ImGui::PushFont(mainfont_heading);
        ImVec2 text_size = ImGui::CalcTextSize("Voronoi Diagram Playground");
        ImGui::PopFont();
        ImGui::PushFont(iconfont);
        ImVec2 icon_size = ImGui::CalcTextSize(u8"\uE016");
        ImGui::PopFont();

        float total_text_width = text_size.x + icon_size.x + 5.0f; // Add some padding between text and icon

        float button_width = 200.0f; // Fixed button width
        float button_height = 40.0f; // Fixed button height
        float spacing = 20.0f; // Spacing between elements

        // Calculate total height of content (text + spacing + buttons)
        float total_height = text_size.y + spacing + button_height + spacing + button_height;

        // Calculate starting Y position to center content vertically
        float start_y = (window_size.y - total_height) * 0.5f;

        // Calculate the X position to center the text and icon
        float text_x = (window_size.x - total_text_width) * 0.5f;

        ImGui::SetCursorPos(ImVec2(text_x, start_y));
        ImGui::PushFont(mainfont_heading);
        ImGui::Text("Voronoi Diagram Playground"); // Pamphlet icon
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::PushFont(iconfont);
        ImGui::Text("%s", u8"\uE016"); // Unicode for Dripicons icon            ImGui::PopFont();
        ImGui::PopFont();
        // Add spacing
        start_y += text_size.y + spacing;

        // Center "Start a New Diagram" button horizontally and position vertically
        float button_x = (window_size.x - button_width) * 0.5f;
        ImGui::SetCursorPos(ImVec2(button_x, start_y));
        ImGui::PushFont(iconfont);
        ImGui::Text("%s", u8"\uE03F"); // Unicode for Dripicons icon            ImGui::PopFont();
        ImGui::PopFont();
        ImGui::SameLine();
        if (ImGui::Button("Create Diagram", ImVec2(button_width, button_height))) { // Rocket icon
            std::cout << "Create Diagram button clicked!" << std::endl;
        }

        // Add spacing
        start_y += button_height + spacing;

        // Center "Load a New Diagram" button horizontally and position vertically
        ImGui::SetCursorPos(ImVec2(button_x, start_y));
                ImGui::PushFont(iconfont);
        ImGui::Text("%s", u8"\uE055"); // Unicode for Dripicons icon            ImGui::PopFont();
        ImGui::PopFont();
        ImGui::SameLine();
        if (ImGui::Button("Load Diagram", ImVec2(button_width, button_height))) { // Upload icon
            std::cout << "Load Diagram button clicked!" << std::endl;
        }

        ImGui::End();

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

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}