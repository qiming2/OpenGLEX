#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

/*
	Functions handling files
*/

std::string readAll(const char* filePath)
{
	std::ifstream in;
	in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string content;
	try
	{
		// open the file
		in.open(filePath);
		// open a stream to receive contents
		std::stringstream shaderStream;
		shaderStream << in.rdbuf();
		// Close file after finishing reading
		in.close();
		content = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "FILE::READ_FAILED\n" << " File: " << __FILE__  << " Passed in: " << filePath << std::endl;
	}
	return content;
}