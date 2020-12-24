
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "m_Shader.h"
#include "Texture.h"
#include "Scene\Scene.h"
#include "Scene\TextureScene.h"



// Width and height
const int width = 1080;
const int height = 1080;
const char* glsl_version = "#version 130";

// callback when window is resized
static void frame_buffer_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);
static void processMix(GLFWwindow* window, float& mix);
static void glfw_error_callback(int error, const char* description);
static GLFWwindow* init();
static void terminate(GLFWwindow* window);

int main(void)
{
    GLFWwindow* window = init();

    {
        Renderer renderer;
        Scene::Scene* currentScene = nullptr;
        Scene::SceneMenu* menu = new Scene::SceneMenu(currentScene);

        // Registering different scenes to be experimented
        menu->Register<Scene::TextureScene>("Texture Scene");

        // Set first scene as menu
        currentScene = menu;

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentScene)
            {
                // OnUpdate
                currentScene->OnUpdate(0.0f);

                // OnRendering
                currentScene->OnRendering();
                // Imgui rendering
                ImGui::SetWindowCollapsed(false);
                ImGui::Begin("Scene");

                // Add back button
                if (currentScene != menu && ImGui::Button("Back"))
                {
                    delete currentScene;
                    currentScene = menu;
                }
                currentScene->OnImGuiRendering();

                ImGui::End();
            }
            


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



            processInput(window);
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }


    terminate(window);
    return 0;
}

static void frame_buffer_callback(GLFWwindow* window, int width, int height)
{
    // std::cout << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

// Process input
static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
   
}

static void processMix(GLFWwindow* window, float& mix)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mix += (float)0.05;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mix -= (float)0.05;
    }
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static GLFWwindow* init()
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        exit(-1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Lunar", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    // Set glfw callback
    glfwSetFramebufferSizeCallback(window, frame_buffer_callback);
    glfwSetErrorCallback(glfw_error_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // Set up viewport
    glViewport(0, 0, width, height);
    // vertical synchronization
    glfwSwapInterval(1);


    // Imgui set up
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Set up theme, more themes can be chosen
    ImGui::StyleColorsDark();

    // Setup Platform and renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //Can add different fonts. See example


    if (GLEW_OK != glewInit())
        std::cout << "GlewInit Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    int maxAttri;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttri);
    std::cout << "Max attris allowed: " << maxAttri << std::endl;
    return window;
}

static void terminate(GLFWwindow* window)
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}