
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
#include "Shader.h"
#include "m_Shader.h"
#include "Texture.h"


// Width and height
const int width = 640;
const int height = 480;

// callback when window is resized
void frame_buffer_callback(GLFWwindow* window, int width, int height);

// Process input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, frame_buffer_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // Set up viewport
    glViewport(0, 0, width, height);
    // vertical synchronization
    glfwSwapInterval(1);

    if (GLEW_OK != glewInit())
        std::cout << "GlewInit Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    int maxAttri;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttri);
    std::cout << "Max attris allowed: " << maxAttri << std::endl;
    {
        std::vector<float> vertices =
        {
            // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
        };

        std::vector<unsigned int> indices = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };


        VertexArray va;
        // Creating buffer and getting an index
        VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));
        // Creating index buffer and getting an index
        IndexBuffer ib(indices.data(), indices.size());
        VertexBufferLayout layout;
        // Position Attri
        layout.Push<float>(3);
        // Color Attri
        layout.Push<float>(3);
        // Texture Attri
        layout.Push<float>(2);
        // Add vertexbuffer using layout object functions
        va.AddBuffer(vb, layout);

        // Use relative path
        m_Shader shader("../res/shaders/b_vert.shader", "../res/shaders/b_frag.shader");
        shader.Bind();
        shader.SetInt("texture1", 0);

        shader.SetInt("texture2", 1);
        
        // shader.SetUniform4f("u_Color", 0.3f, 0.7f, 0.8f, 1.0f);
        // shader.SetUniform4f("a_Color", 0.1f, 0.1f, 0.1f, 1.0f);
       

        Renderer renderer;
        
        

        Texture texture1("../res/Texture/container.jpg", GL_TEXTURE0);
        Texture texture2("../res/Texture/awesomeface.png", GL_TEXTURE1);
        texture1.Bind();
        texture2.Bind();
        float r = 0.0f;
        float increment = 0.01f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            
            // Sending uniform needs to be after the shader program has been bound
            // and it needs to be the right shader program definitely
            
            // shader.SetUniform4f("u_Color", r, 0.6f, 0.8f, 1.0f);
            texture1.Bind();
            texture2.Bind();
            shader.Bind();


            // mode; start index of the enabled arrays, number of indices to be rendered
            // type of indices in this case unsigned int, and pointer to the array of
            // indices
            renderer.Draw(va, ib);
           

            if (r > 1.0f || r < 0.0f)
                increment = -increment;

            r += increment;


            processInput(window);
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}

void frame_buffer_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}