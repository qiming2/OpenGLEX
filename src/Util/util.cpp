#include "util.h"

std::vector<std::string> k_util::split(std::string s, const std::string& delim)
{
	std::vector<std::string> ret;
	size_t pos;
	while ((pos = s.find(delim)) != std::string::npos) {
		ret.emplace_back(s.substr(0, pos));
		s.erase(0, pos + delim.length());
	}
	ret.emplace_back(s);
	return ret;
}