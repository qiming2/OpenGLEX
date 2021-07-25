#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(unsigned int size) {
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// automatically link to binding point 0
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_RendererID, 0, size);
}

void UniformBuffer::Subdata(unsigned int offset, unsigned int size, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetbindingPoint(unsigned int offset, unsigned int bindPoint, unsigned int size) {
	glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint, m_RendererID, offset, size);
}

void UniformBuffer::Bind() {
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
}

void UniformBuffer::Delete() {
	glDeleteBuffers(1, &m_RendererID);
}

void UniformBuffer::Unbind() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}