#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

/*
	Functions handling files
*/

std::string readAll(const char* filePath);
std::vector<std::string> concatFacePath(const std::string path);