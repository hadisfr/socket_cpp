#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>

std::string itos(int);
std::vector<std::string> split(std::string text, std::string delimiter);
std::string trim(std::string);
void log(std::string);

#endif