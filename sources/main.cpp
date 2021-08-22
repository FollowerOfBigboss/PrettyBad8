#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "vm/vm.h"
#include "emu/emu.h"

#include <iostream>

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwResizeCallback(GLFWwindow* window, int width, int height);
void glfwJoystickInput();

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

Emu emu;

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
    
    emu.init();
    glfwSwapInterval(emu.Vsync);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiBeginFrame();
        glfwJoystickInput();
        emu.run();
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
        if (emu.pmode.pressed == false)
        {
            emu.keymap[emu.pmode.keytochanged] = key;
            emu.pmode.pressed = true;
        }

        if (key == GLFW_KEY_F10)
        {
            emu.ShouldDrawMenuBar = !emu.ShouldDrawMenuBar;
        }

        if (key == GLFW_KEY_F2)
        {
            // Save State
            emu.SaveState();
        }

        if (key == GLFW_KEY_F3)
        {
            // Load State
            emu.LoadState();
        }

        if (emu.CurrentInput == EmuInput::Keyboard)
        {
            emu.presskey(key);
        }
    }

    if (action == GLFW_RELEASE)
    {
        if (emu.CurrentInput == EmuInput::Keyboard)
        {
            emu.releasekey(key);
        }
    }
}

void glfwResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfwJoystickInput()
{
    if (emu.CurrentInput == EmuInput::Controller)
    {

        if (glfwJoystickPresent(GLFW_JOYSTICK_1))
        {
            memset(emu.vm.Key, 0, sizeof(uint8_t) * 16);
            emu.handlecontroller();

        }
    }

}