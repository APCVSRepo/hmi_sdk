#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <iostream>
#include <string>
#include <vector>

std::string& trim(std::string &s);

void split(const std::string& src, const std::string& separator, std::vector<std::string>& dest);

#endif // STRINGUTIL_H

