#pragma once

// Abstracted VertexBuffer class
class VertexBuffer
{
public:
	// size for the total bytes not the count
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	
	void operator=(const VertexBuffer& vb2);
	~VertexBuffer();
	void ReMap(const void* data, unsigned int size);
	void Bind() const;
	void Unbind() const;
	void Delete();
private:
	unsigned int m_RendererID;
	bool binded = false;
};
