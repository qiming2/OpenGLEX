
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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


    float position[6] = 
    {
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

    // Load shaders from file

    
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    std::cout << "Vertex: " << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment: " << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

    // bind shader or select the shader to use
    glUseProgram(shader);

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
    // Free memory of the shader program
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}