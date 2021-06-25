
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
#include "Common.h"
#include "Scene\Scene.h"
#include "Scene\TextureScene.h"
#include "Scene\TransformScene.h"
#include "Scene\CoordinateScene.h"
#include "Scene\CameraScene.h"
#include "Scene\LightScene.h"
#include "Scene\LightTypeScene.h"




// Width and height
int Width = 1080;
int Height = 1080;
const char* glsl_version = "#version 130";

// callback when window is resized
static void frame_buffer_callback(GLFWwindow* window, int Width, int height);
static void processInput(GLFWwindow* window);
static void processMix(GLFWwindow* window, float& mix);
static void glfw_error_callback(int error, const char* description);
static GLFWwindow* init();
static void terminate(GLFWwindow* window);
GLFWwindow* Window = nullptr;
float DeltaTime = 0.0f;

int main(void)
{
    Window = init();

    {
        //Renderer renderer;
        Scene::Scene* currentScene = nullptr;
        Scene::SceneMenu* menu = new Scene::SceneMenu(currentScene);

        // Registering different scenes to be experimented
        menu->Register<Scene::TextureScene>("Texture Scene");
        menu->Register<Scene::TransformScene>("Transform Scene");
        menu->Register<Scene::CoordinateScene>("Coordinate Scene");
        menu->Register<Scene::CameraScene>("Camera Scene");
        menu->Register<Scene::LightScene>("Light Scene");
        menu->Register<Scene::LightTypeScene>("Light Type Scene");
        // Set first scene as menu
        currentScene = menu;
        double prevTime = glfwGetTime();
        double currTime = 0.0;
        while (!glfwWindowShouldClose(Window))
        {
            /* Render here */
            //renderer.Clear();
            // Start the Dear ImGui frame
            currTime = glfwGetTime();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentScene)
            {
                
                // Imgui rendering
                ImGui::SetWindowCollapsed(false);
                ImGui::Begin("Scene");
				// OnUpdate
				// Todo Need to change deltaTime
                DeltaTime = (float) currTime - prevTime;
				currentScene->OnUpdate(DeltaTime);

				// OnRendering
				currentScene->OnRendering();
                // Add back button
                if (currentScene != menu && ImGui::Button("Back"))
                {
                    delete currentScene;
                    currentScene = menu;
                }
                currentScene->OnImGuiRendering();
                ImGui::Text("Frame per second: %f", 1.0f / DeltaTime);
                ImGui::End();
            }
            prevTime = currTime;
            


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



            processInput(Window);
            /* Swap front and back buffers */
            glfwSwapBuffers(Window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }


    terminate(Window);
    return 0;
}

static void frame_buffer_callback(GLFWwindow* Window, int width, int height)
{
    // std::cout << width << " " << height << std::endl;
    Width = width;
    Height = height;
    glViewport(0, 0, width, height);
}

// Process input
static void processInput(GLFWwindow* Window)
{
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Window, true);
   
}

static void processMix(GLFWwindow* Window, float& mix)
{
    if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mix += (float)0.05;
    }

    if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
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
    window = glfwCreateWindow(Width, Height, "Lunar", NULL, NULL);
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
    glViewport(0, 0, Width, Height);
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

static void terminate(GLFWwindow* Window)
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(Window);
    glfwTerminate();
}