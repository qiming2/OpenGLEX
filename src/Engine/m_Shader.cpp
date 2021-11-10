#include <string>
#include <gl\glew.h>
#include <iostream>
#include "FileUtil.h"
#include "m_Shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

m_Shader::m_Shader(const char* vertexPath, const char* fragmentPath):
	vPath(vertexPath),
	fPath(fragmentPath)
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
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << vertexPath << "\n" << fragmentPath << "\n" << log << std::endl;
	}
	// Delete Shader after successful linkage
	glDeleteShader(vertex);
	glDeleteShader(frag);
	
	m_RendererID = program;
	Unbind();
}

m_Shader::m_Shader(const char* vertexPath, const char* fragPath, const char* geoPath):
	vPath(vertexPath),
	fPath(fragPath),
	gPath(geoPath)
{
	// read all shader code from files

	// Since std::string is a temporary object, if using someString.c_str()
	// the char* might point to garbage value 
	/*const char* vcode = readAll(vPath).c_str();
	const char* fcode = readAll(fPath).c_str();
	const char* gcode = readAll(gPath).c_str();*/
	
	// Right way is to copy 
	const std::string& vCode = readAll(vertexPath);
	const std::string& fCode = readAll(fragPath);
	const std::string& gCode = readAll(geoPath);
	const char* vcode = vCode.c_str();
	const char* fcode = fCode.c_str();
	const char* gcode = gCode.c_str();

	unsigned int vShader = CompileShader(vcode, GL_VERTEX_SHADER);
	unsigned int fShader = CompileShader(fcode, GL_FRAGMENT_SHADER);
	unsigned int gShader = CompileShader(gcode, GL_GEOMETRY_SHADER);

	unsigned int program = glCreateProgram();

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	// Add our geometry shader
	glAttachShader(program, gShader);

	glLinkProgram(program);
	glValidateProgram(program);

	int success;
	char log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << vPath << "\n" << fPath << "\n" << gPath << "\n" << log << std::endl;
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glDeleteShader(gShader);
	
	m_RendererID = program;
	Unbind();
}

m_Shader::~m_Shader()
{

}

void m_Shader::Delete() const {
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

void m_Shader::SetVec3fv(const std::string& name, const glm::vec3& val)
{
	int location;
	if ((location = GetUniformLocation(name)) < 0)
	{
		return;
	}

	glUniform3fv(location, 1, glm::value_ptr(val));
}

void m_Shader::SetVec3fv(const std::string& name, const float v1, float v2, float v3)
{
	int location;
	if ((location = GetUniformLocation(name)) < 0)
	{
		return;
	}

	glUniform3f(location, v1, v2, v3);
}

void m_Shader::SetFloat(const std::string& name, float v0)
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

void m_Shader::SetMat4fv(const std::string& name, const glm::mat4& val)
{
	int location;
	if ((location = GetUniformLocation(name)) < 0)
	{
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
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

void m_Shader::SetUniformBlock(const std::string& name, unsigned int bindPoint) {
	int location;
	if ((location = GetUniformBlock(name)) < 0) {
		return;
	}
	glUniformBlockBinding(m_RendererID, location, bindPoint);
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
		else if (type == GL_GEOMETRY_SHADER) typeS = "GEOMETRY";
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
		std::cout << "UNIFORM LOCATION UNKNOWN\n" << "File: " << __FILE__ << " AT LINE: " << __LINE__ << " Unknown Name: " << name << " File path: " << "\n" << vPath << "\n" << fPath << "\n" << gPath << std::endl;
	}
	m_uniformLocation[name] = location;
	return location;
	
}

int m_Shader::GetUniformBlock(const std::string& name)
{
	std::unordered_map<std::string, int>::iterator it;
	// Find out whether it is in the cache
	if ((it = m_uniformLocation.find(name)) != m_uniformLocation.end())
	{
		return it->second;
	}

	// If it is not in the cache, we retrive it and store it in the cache
	int location = glGetUniformBlockIndex(m_RendererID, name.c_str());
	if (location == -1)
	{
		std::cout << "UNIFORM LOCATION UNKNOWN\n" << "File: " << __FILE__ << " AT LINE: " << __LINE__ << " Unknown Name: " << name << " File path: " << vPath << "\n" << fPath << "\n" << gPath << std::endl;
	}
	m_uniformLocation[name] = location;
	return location;

}