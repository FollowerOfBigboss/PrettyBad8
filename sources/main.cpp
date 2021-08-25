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
void glfwJoystickInput();
void ImGuiBeginFrame();
void ImGuiEndFrame();


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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiBeginFrame();
        glfwJoystickInput();
        g_Emu.run();
#ifdef PDEBUG
        ImGui::ShowDemoWindow();
#endif
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

void glfwJoystickInput()
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
