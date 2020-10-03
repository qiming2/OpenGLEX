
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    // Create the shader object with the corresponding type like Vertex shader or fragment shader
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // id: the shader whose source code would be replaced
    // 
    glShaderSource(id, 1, &src, nullptr);
}

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


    float position[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };
    // Creating buffer and getting an index
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    // bind buffer since we are going to work on it
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Pass and store the data t othe buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

    // Creates the vertex attribute like position, color, etc
    // index of the attri, how many components are the attribute, type of the component,
    // bytes away from next the same attribute, bytes away from the next attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    // Needs to enable the vertex attri
    glEnableVertexAttribArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        // mode; start index of the enabled arrays, number of indices to be rendered
        glDrawArrays(GL_TRIANGLES, 0, 3);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}