#pragma once

#include <GL/glew.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>

#define ASSERT(x) if (!(x)) __debugbreak();
// Abstracted renderer class

// For debugging purpose using conventional way

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);



// Renderer can be singleton or can be instantiated as
// multiple instances
class Renderer
{
public:
    void Clear() const;
    // The main function is the Draw function
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};