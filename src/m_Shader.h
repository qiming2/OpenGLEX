#pragma once
#include <string>
#include <unordered_map>

class m_Shader
{
public:
	m_Shader(const char* vertexPath, const char* fragmentPath);
	~m_Shader();

	void Bind() const;
	void Unbind() const;
	// Set uniform variable
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	// Render ID
	unsigned int m_rID;
	// Uniform location caches for getting or setting uniform values
	std::unordered_map<std::string, int> m_uniformLocation;

	// Create and compile shader
	unsigned int CompileShader(const char* code, unsigned int type);

	int GetUniformLocation(const std::string& name);
};