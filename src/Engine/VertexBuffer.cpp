#include <iostream>
#include "VertexBuffer.h"
#include "Renderer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    binded = true;
    GLCall(glGenBuffers(1, &m_RendererID));

    // bind buffer since we are going to work on it
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // Pass and store the data to the buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer() {
    GLCall(glGenBuffers(1, &m_RendererID));
}

void VertexBuffer::ReMap(const void* data, unsigned int size) {
    if (!m_RendererID) {
        //std::cout << "VERTEXBUFFER::ERROR" << std::endl;
        GLCall(glGenBuffers(1, &m_RendererID));
    }
    binded = true;
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::operator=(const VertexBuffer& vb2) {
    m_RendererID = vb2.m_RendererID;
}

VertexBuffer::~VertexBuffer()
{
    
}

void VertexBuffer::Delete() {
    binded = false;
    GLCall(glDeleteBuffers(1, &m_RendererID));
    m_RendererID = 0;
}

void VertexBuffer::Bind() const
{   
    if (!binded) {
        std::cout << "VERTEXBUFFER::ERROR::NO BINDED DATA" << std::endl;
        return;
    }
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}