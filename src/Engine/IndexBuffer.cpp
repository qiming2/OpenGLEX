#include <iostream>
#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    binded = true;
    GLCall(glGenBuffers(1, &m_RendererID));

    // bind buffer since we are going to work on it
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    // Pass and store the data to the buffer
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer() {
    GLCall(glGenBuffers(1, &m_RendererID));
}

void IndexBuffer::ReMap(const unsigned int* data, unsigned int count)
{
    m_Count = count;
    if (!m_RendererID)  {
        GLCall(glGenBuffers(1, &m_RendererID));
    }
    binded = true;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    
}

void IndexBuffer::Delete() {
    //std::cout << "Delete IndexBuffer" << std::endl;
    binded = false;
    GLCall(glDeleteBuffers(1, &m_RendererID));
    m_RendererID = 0;
}

void IndexBuffer::Bind() const
{
    if (!binded) {
        std::cout << "INDEXBUFFER::ERROR::NO BINDED DATA" << std::endl;
        return;
    }
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}