#pragma once


// Abstracted IndexBuffer class
class IndexBuffer
{
public:
	// count for the specific number of "elements"
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void Delete() const;
	inline unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};
