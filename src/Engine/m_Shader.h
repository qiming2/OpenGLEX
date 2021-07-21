#pragma once
#include <string>
#include <unordered_map>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class m_Shader
{
public:
	m_Shader(const char* vertexPath, const char* fragmentPath);
	m_Shader(const char* vPath, const char* fPath, const char* gPath);
	~m_Shader();
	

	void Bind() const;
	void Unbind() const;
	void Delete() const;
	// Set uniform variable
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void SetInt(const std::string& name, int v0);
	void SetFloat(const std::string& name, float v0);
	void SetMat4fv(const std::string& name, const float* ptr);
	void SetVec3fv(const std::string& name, const float* ptr);
	void SetMat3fv(const std::string& name, const float* ptr);
	void SetMat4fv(const std::string& name, const glm::mat4& val);
	void SetVec3fv(const std::string& name, float v1, float v2, float v3);
	void SetVec3fv(const std::string& name, const glm::vec3& val);
private:
	// Render ID
	unsigned int m_RendererID;
	// Uniform location caches for getting or setting uniform values
	std::unordered_map<std::string, int> m_uniformLocation;

	// Create and compile shader
	unsigned int CompileShader(const char* code, unsigned int type);

	int GetUniformLocation(const std::string& name);
};