#pragma once


// Abstracted IndexBuffer class
class IndexBuffer
{
public:
	// count for the specific number of "elements"
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer();
	~IndexBuffer();

	void ReMap(const unsigned int* data, unsigned int count);
	void Bind() const;
	void Unbind() const;
	void Delete();
	inline unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
	bool binded = false;
};
