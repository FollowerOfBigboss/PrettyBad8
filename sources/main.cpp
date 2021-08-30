#include <iostream>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "vm/vm.h"
#include "emu/emu.h"

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwResizeCallback(GLFWwindow* window, int width, int height);
void JoystickInput();

void ImGuiBeginFrame();
void ImGuiEndFrame();
void fpsoverlay();

Emu g_Emu;

int main()
{
    GLFWwindow* window;
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "PrettyBad8", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, &glfwKeyCallback);
    glfwSetWindowSizeCallback(window, &glfwResizeCallback);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    
    g_Emu.init();
    glfwSwapInterval(g_Emu.b_Vsync);

    while (!glfwWindowShouldClose(window) && g_Emu.b_EmuRun == true)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiBeginFrame();
        JoystickInput();
        g_Emu.run();

#ifdef PDEBUG
        ImGui::ShowDemoWindow();
#endif

        if (g_Emu.b_ShowFPS)
            fpsoverlay();
        
        ImGuiEndFrame();
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (g_Emu.pmode.pressed == false)
        {
            g_Emu.keymap[g_Emu.pmode.keytochanged] = key;
            g_Emu.pmode.pressed = true;
        }

        if (key == GLFW_KEY_F10)
        {
            g_Emu.b_ShouldDrawMenuBar = !g_Emu.b_ShouldDrawMenuBar;
        }

        if (key == GLFW_KEY_F2)
        {
            // Save State
            g_Emu.SaveState();
        }

        if (key == GLFW_KEY_F3)
        {
            // Load State
            g_Emu.LoadState();
        }

        if (g_Emu.CurrentInput == EmuInput::Keyboard)
        {
            g_Emu.presskey(key);
        }
    }

    if (action == GLFW_RELEASE)
    {
        if (g_Emu.CurrentInput == EmuInput::Keyboard)
        {
            g_Emu.releasekey(key);
        }
    }
}

void glfwResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void JoystickInput()
{
    if (g_Emu.CurrentInput == EmuInput::Controller)
    {
        if (glfwJoystickPresent(GLFW_JOYSTICK_1))
        {
            g_Emu.handlecontroller();
        }
    }

}

void ImGuiBeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiEndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void fpsoverlay()
{
    const float PAD = 10.0f;
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", 0, window_flags))
    {
        ImGui::Text("%.1f FPS",  ImGui::GetIO().Framerate);

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (ImGui::MenuItem("Close")) g_Emu.b_ShowFPS = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}