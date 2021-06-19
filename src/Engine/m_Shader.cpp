#include <string>
#include <gl\glew.h>
#include <iostream>
#include "FileUtil.h"
#include "m_Shader.h"

m_Shader::m_Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vCode = readAll(vertexPath);
	std::string fCode = readAll(fragmentPath);
	const char* vShaderCode = vCode.c_str();
	const char* fShaderCode = fCode.c_str();
	// Compile two different types of shader respectively
	unsigned int vertex = CompileShader(vShaderCode, GL_VERTEX_SHADER);
	unsigned int frag = CompileShader(fShaderCode, GL_FRAGMENT_SHADER);

	// Create our shader program that needs to contain both shaders
	unsigned int program = glCreateProgram();

	// Attach and Link
	glAttachShader(program, vertex);
	glAttachShader(program, frag);
	// Linking makes sure our in and out variables are reasonable
	// in each programmable stage of rendering pipe
	glLinkProgram(program);
	glValidateProgram(program);

	int success;
	char log[512];
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		std::cout << "ERROR::SHADER::PROGRAMM::LINKING_FAILED\n" << log << std::endl;
	}
	// Delete Shader after successful linkage
	glDeleteShader(vertex);
	glDeleteShader(frag);
	
	m_RendererID = program;
	Unbind();
}

m_Shader::~m_Shader()
{
	glDeleteProgram(m_RendererID);
}

void m_Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void m_Shader::Unbind() const
{
	glUseProgram(0);
}

void m_Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location;
	// Find whether the variable exists
	if ((location = GetUniformLocation(name)) < 0)
	{
		return;
	}
	
	glUniform4f(location, v0, v1, v2, v3);
}

void m_Shader::SetVec3fv(const std::string& name, const float* ptr) {
	int location;
	if ((location = GetUniformLocation(name)) < 0) {
		return;
	}

	glUniform3fv(location, 1, ptr);
}

void m_Shader::Setfloat(const std::string& name, float v0)
{
	int location;
	if ((location = GetUniformLocation(name)) < 0)
	{
		return;
	}
	glUniform1f(location, v0);
}

void m_Shader::SetMat3fv(const std::string& name, const float* ptr)
{
	int location;
	if ((location = GetUniformLocation(name)) < 0)
	{
		return;
	}
	glUniformMatrix3fv(location, 1, GL_FALSE, ptr);
}

void m_Shader::SetMat4fv(const std::string& name, const float* ptr)
{
	int location;
	if ((location = GetUniformLocation(name)) < 0)
	{
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, ptr);
}

void m_Shader::SetInt(const std::string& name, int v0)
{
	
	int location;
	if ((location = GetUniformLocation(name)) < 0)
	{
		return;
	}
	glUniform1i(location, v0);
}

unsigned int m_Shader::CompileShader(const char* code, unsigned int type)
{
	unsigned int id;
	int success;
	char log[512];

	// Create Shader of type (GL_VERTEX_SHADER and GL_FRAGMENT_SHADER are usually what we are working on)
	id = glCreateShader(type);
	// The last one specifies an array of string lengths
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::string typeS;
		if (type == GL_VERTEX_SHADER) typeS = "VERTEX";
		else if (type == GL_FRAGMENT_SHADER) typeS = "FRAGMENT";
		else typeS = "UNKNOWN";

		/*	1: id of shader
			2: log buffer size
			3: length of returned log buffer
			4: pointer to log buffer for holding message
		*/
		glGetShaderInfoLog(id, sizeof(log), NULL, log);
		std::cout << "ERROR::" << typeS << "SHADER::COMPILATION_FAILED\n" << log << " File: " << __FILE__ << std::endl;
	}

	return id;
}

int m_Shader::GetUniformLocation(const std::string& name)
{
	std::unordered_map<std::string, int>::iterator it;
	// Find out whether it is in the cache
	if ((it = m_uniformLocation.find(name)) != m_uniformLocation.end())
	{
		return it->second;
	}

	// If it is not in the cache, we retrive it and store it in the cache
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
	{
		std::cout << "UNIFORM LOCATION UNKNOWN\n" << "File: " << __FILE__ << " AT LINE: " << __LINE__ << std::endl;
	}
	m_uniformLocation[name] = location;
	return location;
	
}