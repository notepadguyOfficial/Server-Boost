#pragma once
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>

std::vector<int> parse(const std::string& version);
bool check_version(const std::vector<int>& v1, const std::vector<int>& v2);
void print_version(const std::vector<int>& version);
