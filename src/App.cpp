
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex2f(0.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.5f, -0.5f);
        glEnd();
        //glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
        //glColor3f(1.0f, 0.0f, 0.0f); // Red
        //glVertex2f(-0.5f, -0.5f);    // x, y
        //glVertex2f(0.5f, -0.5f);
        //glVertex2f(0.5f, 0.5f);
        //glVertex2f(-0.5f, 0.5f);
        //glEnd();
        glBegin(GL_TRIANGLES);          // Each set of 3 vertices form a triangle
        //glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.1f, -0.6f);
        glVertex2f(0.7f, -0.6f);
        glVertex2f(0.4f, -0.1f);

        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(0.3f, -0.4f);
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex2f(0.9f, -0.4f);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.6f, -0.9f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}