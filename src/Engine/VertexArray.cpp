#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

// In core profile, we need to bound a vertex array object
// to a vertexattriLayout
// we can create different vaos for each of geometry we want to
// draw, and we can basically switch between vaos instead of
// needing to bind different buffers and attributelayout every time
// we want to draw something differnt
// Performance wise, it is difficult to detemine which way is better

VertexArray::VertexArray():
	attriLoc(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{

	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		Bind();
		const auto& element = elements[i];
		// Needs to enable the vertex attri
		GLCall(glEnableVertexAttribArray(attriLoc));
		// Creates the vertex attribute like position, color, etc
		// index of the attri, how many components are the attribute, type of the component,
		// bytes away from next the same attribute, bytes away from the next attribute
		GLCall(glVertexAttribPointer(attriLoc, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		attriLoc++;
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
