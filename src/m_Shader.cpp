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
	m_rID = program;
}

m_Shader::~m_Shader()
{
	glDeleteProgram(m_rID);
}

void m_Shader::Bind() const
{
	glUseProgram(m_rID);
}

void m_Shader::Unbind() const
{
	glUseProgram(0);
}

void m_Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location;
	// Find whether the variable exists
	if (location = GetUniformLocation(name) < 0)
	{
		return;
	}
	glUniform4f(location, v0, v1, v2, v3);
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
	if ((it = m_uniformLocation.find(name)) != m_uniformLocation.end())
	{
		return it->second;
	}
	int location = glGetUniformLocation(m_rID, name.c_str());
	if (location == -1)
	{
		std::cout << "UNIFORM LOCATION UNKNOWN\n" << "File: " << __FILE__ << std::endl;
	}
	m_uniformLocation[name] = location;
	return location;
	
}