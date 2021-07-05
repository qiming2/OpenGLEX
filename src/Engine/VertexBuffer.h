#pragma once

// Abstracted VertexBuffer class
class VertexBuffer
{
public:
	// size for the total bytes not the count
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void Delete() const;
private:
	unsigned int m_RendererID;
	
};
