#pragma once
#include <GL/glew.h>
#include "Common.h"

class UniformBuffer
{
public:
	UniformBuffer(unsigned int size);
	
	void Bind();
	void Delete();
	void Unbind();
	void Subdata(unsigned int offset, unsigned int size, void* data);
	void SetbindingPoint(unsigned int size, unsigned int bindPoint, unsigned int offset);
private:
	unsigned int m_RendererID;

};

