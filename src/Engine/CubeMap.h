#pragma once
#include "Common.h"

// Simple CubeMap wrapper class
class CubeMap
{
public:
	CubeMap(const std::vector<std::string>& paths, GLenum activeID);
	~CubeMap();

	void Delete();
	void Bind();
	void Unbind();
	void SetActiveID(GLenum activeID);
private:
	unsigned int m_RendererID;
	bool binded = false;
	GLenum m_activeID;
};

