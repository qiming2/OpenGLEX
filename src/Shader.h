#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	// Caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
	// Compile the specific shader
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int GetUniformLocation(const std::string& name);
};