# pragma once

#include "core.hpp"

std::string get_input(void);
void print(std::string str);
void cross_platform_sleep(int ms);
std::vector<std::string> split(std::string str, char delim);
bool has_any_digits(const std::string& s);