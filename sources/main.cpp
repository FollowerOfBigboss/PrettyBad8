#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include "vm/vm.h"
#include "vm/draw.h"
#include "vm/ui.h"
#include "vm/debugger.h"

#include <iostream>


void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwResizeCallback(GLFWwindow* window, int width, int height);

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

VM VM_CHIP;

int main()
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

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

    CDQuads drquads;

    EmuUi::vm.init();
    EmuUi::vm.loadrom("pong");

    EmuUi::EDebugger.attach(EmuUi::vm);
    EmuUi::EDebugger.GetDissassembly();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        EmuUi::DrawMenuBar();
        EmuUi::DrawDebuggerStuf();
        
        EmuUi::EmuLoop();

        ImGui::ShowDemoWindow();
        
        drquads.update(EmuUi::vm);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);

    }

    EmuUi::EDebugger.deattach();
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