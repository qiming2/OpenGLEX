# Purpose of OpenGLEX

The purpose of this project is to render scenes, objects, lights and other components with different rendering techniques and lighting models.

## Open GL Rendering Engine By Qiming Guan

Currently building infrastructures for the engine. Upon finishing the basic infrastructure, user may load models, create your own shaders, load textures
and even manipulate any code you would like to experiment your own rendering methods.

<br>

////////////////////////////////// Learning Notes ///////////////////////////////////

### Set up OpenGL workplace

- Find a way to create valid GL context like glfw package
- Then in order to use the library that graphics driver provides, we can use a
  library that can fetch corresponding openGL library according to different OS
- Download those packages, in visual studio, we need to add paths to those header
  files in properties of the solution in order to compile them
- We also need to link libraries we have downloaded from the web. (Add path to the solution in linker property, and usually we want to link static library)
- If there are other linker errors, we need to search which else libraries are needed to compile the program

### OpenGL Learning

#### Before actual writing GL code

- Create a window with GLFW or corresponding window according to OS
- Make window to the context current if we want to work on it
- `glGetString(GL_VERSION)` is a good way to gl version and which graphics card we are using

#### Simple use case

- we need to create buffer and store the index we are given:
```
// Creating buffer and getting an index
unsigned int buffer;

glGenBuffers(1, &buffer);
```
- Bind buffer to GL_ARRAY_BUFFER and pass data into it
```
float position[6] =
    {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };
// bind buffer since we are going to work on it
glBindBuffer(GL_ARRAY_BUFFER, buffer);

// Pass and store the data t othe buffer
glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);
```
- Specify Vertex attribute! This is very important if we want to write shader programs that can access the data we provide.
```
// Creates the vertex attribute like position, color, etc
// index of the attri, how many components are the attribute, type of the component,
// bytes away from next the same attribute, bytes away from the next attribute
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

// Needs to enable the vertex attri
glEnableVertexAttribArray(0);
```
Be sure to enable the attributes in the end, currently we only have defined one type of attribute which is position, we should write as many lines of attributePointer as the number of attributes in the vertex buffer we have defined, which currently refers to position.

- Next we need to start writing a basic shader program. First we need to have our shader source code: we have two options. We either write all the code in a string in the cpp file (which is definitely not efficient), and we write our shader source code in a separate file and parse them into the c string!
```
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};
ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
std::cout << "Vertex: " << std::endl;
std::cout << source.VertexSource << std::endl;
std::cout << "Fragment: " << std::endl;
std::cout << source.FragmentSource << std::endl;
```
Since we have two different shaders we want to write, we define a struct to store each of source code

- Then we need to actually create the shaders in our opengl, and link them together and bind them to shader program.
```
unsigned int program = glCreateProgram();

// CompileShader is a method for use create a shader and compile them gpu
unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

// Overview of CompileShader(ShaderType, source) func:
// type EX: GL_VERTEX_SHADER
unsigned int id = glCreateShader(type);
const char* src = source.c_str();

// id: the shader whose source code would be replaced
// size: how many number of strings in the array of strings
// the pointer to the array of strings
// string array: pointer to an array of integers specifying the length of each string
// array of length of each string: nullptr means that each string is terminated with null
glShaderSource(id, 1, &src, nullptr);
glCompileShader(id);

// Compile shader end
// Attach shader objects to the program
// Then we link them together
// For details, read the documentation: docs.GL
glAttachShader(program, vs);
glAttachShader(program, fs);
glLinkProgram(program);
glValidateProgram(program);
// bind shader or select the shader to use
glUseProgram(shader);
```

- Then we can use index buffer in order to save memory, for example, we have two triangles that share some of the same vertexes. In this case, we don't need to store duplicate data but we'd rather use the same data again
```
unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
};
// Creating index buffer and getting an index
unsigned int ibo;
glGenBuffers(1, &ibo);
// bind buffer since we are going to work on it
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
// Pass and store the data to the buffer
glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
// Draw call with indices array
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
```
The code can change during development, and this is a showcase of basic usage of opengl.
Later development would have corresponding detailed comments in code,
you can check it out in the actual cpp files.
