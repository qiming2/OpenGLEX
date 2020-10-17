
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // vertical synchronization
    glfwSwapInterval(1);

    if (GLEW_OK != glewInit())
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float position[] =
        {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };


        VertexArray va;
        // Creating buffer and getting an index
        VertexBuffer vb(position, 4 * 2 * sizeof(float));
        // Creating index buffer and getting an index
        IndexBuffer ib(indices, 6);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.3f, 0.7f, 0.8f, 1.0f);
        // unbind
        vb.Unbind();
        va.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.01f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
           

            // Don't need to take these actions since we have
            // vertex array object, which helps reduce a lot of
            // operations
            // However this could be an option for potential better performance
            /*glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
            glEnableVertexAttribArray(0);*/

            // Draw call loop in order to draw different objects
            // We definitely need to bind different buffer, indices,
            // vertexAttri and shader if necessary
            // However vertex buffer is more useful for storing
            // different attributeLayout into vertex buffers
            
            // Sending uniform needs to be after the shader program has been bound
            // and it needs to be the right shader program definitely
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.6f, 0.8f, 1.0f);
            // animate the color


            // mode; start index of the enabled arrays, number of indices to be rendered
            // type of indices in this case unsigned int, and pointer to the array of
            // indices
            renderer.Draw(va, ib, shader);

            if (r > 1.0f || r < 0.0f)
                increment = -increment;

            r += increment;



            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}