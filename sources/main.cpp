#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "vm/vm.h"
#include "vm/ui.h"

#include <iostream>

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwResizeCallback(GLFWwindow* window, int width, int height);


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

int KeyMap[16] =
{
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,

    GLFW_KEY_Q,
    GLFW_KEY_W,
    GLFW_KEY_E,
    GLFW_KEY_R,

    GLFW_KEY_A,
    GLFW_KEY_S,
    GLFW_KEY_D,
    GLFW_KEY_F,

    GLFW_KEY_Z,
    GLFW_KEY_X,
    GLFW_KEY_C,
    GLFW_KEY_V
};

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

    window = glfwCreateWindow(640, 480, "ラストエグザイル", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, &glfwKeyCallback);
    glfwSetWindowSizeCallback(window, &glfwResizeCallback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    
    EmuUi::Init();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiBeginFrame();

        EmuUi::EmuDraw();
        ImGui::ShowDemoWindow();
        
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
        if (key == GLFW_KEY_F10)
            EmuUi::ShouldDrawMenuBar = !EmuUi::ShouldDrawMenuBar;

        for (int i = 0; i < 16; i++)
        {
            if (key == KeyMap[i])
            {
                EmuUi::vm.Key[i] = 1;
            }
        }
    }

    if (action == GLFW_RELEASE)
    {
          for (int i = 0; i < 16; i++)
          {
              if (key == KeyMap[i])
              {
                  EmuUi::vm.Key[i] = 0;
              }
          }

    }
}


void glfwResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}