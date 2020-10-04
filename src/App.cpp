
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath) 
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) 
        {
            if (line.find("vertex") != std::string::npos)
            {
                // Set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // Set mode to fragment
                type = ShaderType::FRAGMENT;

            }
        }
        else
        {
            std::cout << line << std::endl;
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}


static unsigned int CompileShader(unsigned int type, const std::string& source) 
{
    // Create the shader object with the corresponding type like Vertex shader or fragment shader
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // id: the shader whose source code would be replaced
    // size: how many number of strings in the array of strings
    // the pointer to the array of strings
    // pointer to an array of integers specifying the length of each string
    //      nullptr means that each string is terminated with null
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    // TODO: Error handling
    int result;
    // A way to get status for a specific operation that has been done with opengl
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    
    if (result == GL_FALSE)
    {
        int length;
        // getting message from opengl
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length *sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!"
            << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach shader objects to the program
    // Then we link them together
    // For details, read the documentation: docs.GL
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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

        // In core profile, we need to bound a vertex array object
        // to a vertexattriLayout
        // we can create different vaos for each of geometry we want to
        // draw, and we can basically switch between vaos instead of
        // needing to bind different buffers and attributelayout every time
        // we want to draw something differnt

        // Performance wise, it is difficult to detemine which way is better
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Creating buffer and getting an index
        VertexBuffer vb(position, 4 * 2 * sizeof(float));

        // Creating index buffer and getting an index
        IndexBuffer ib(indices, 6);


        // Creates the vertex attribute like position, color, etc
        // index of the attri, how many components are the attribute, type of the component,
        // bytes away from next the same attribute, bytes away from the next attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

        // Needs to enable the vertex attri
        glEnableVertexAttribArray(0);

        // Load shaders from file


        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        std::cout << "Vertex: " << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "Fragment: " << std::endl;
        std::cout << source.FragmentSource << std::endl;

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

        // bind shader or select the shader to use
        glUseProgram(shader);

        // Send data from cpu to gpu or to shader program

        // First retriving location of the specified variable location
        // then we can send data to the location with corresponding data
        // int this case, we have a vec4 which we need to pick glUniform4f to
        // send 4 floats
        int location = glGetUniformLocation(shader, "u_Color");
        ASSERT(location != -1);
        glUniform4f(location, 0.9f, 0.9f, 0.7f, 1.0f);


        // unbind
        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        float r = 0.0f;
        float increment = 0.01f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            GLClearError();

            // Draw call loop in order to draw different objects
            // We definitely need to bind different buffer, indices,
            // vertexAttri and shader if necessary
            // However vertex buffer is more useful for storing
            // different attributeLayout into vertex buffers
            glUseProgram(shader);
            glUniform4f(location, r, 0.6f, 0.8f, 1.0f);

            // Don't need to take these actions since we have
            // vertex array object, which helps reduce a lot of
            // operations
            /*glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
            glEnableVertexAttribArray(0);*/

            glBindVertexArray(vao);
            ib.Bind();
            // animate the color


            // mode; start index of the enabled arrays, number of indices to be rendered
            // type of indices in this case unsigned int, and pointer to the array of
            // indices
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f || r < 0.0f)
                increment = -increment;

            r += increment;



            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        // Free memory of the shader program
        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}