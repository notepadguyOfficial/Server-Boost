#include "Helper.h"

std::vector<int> parse(const std::string& version) {
    std::vector<int> parts;
    std::istringstream iss(version);
    std::string token;
    while (std::getline(iss, token, '.')) {
        parts.push_back(std::atoi(token.c_str()));
    }
    return parts;
}

bool check_version(const std::vector<int>& v1, const std::vector<int>& v2) {
    size_t len = std::max(v1.size(), v2.size());
    for (size_t i = 0; i < len; ++i) {
        int part1 = (i < v1.size()) ? v1[i] : 0;
        int part2 = (i < v2.size()) ? v2[i] : 0;
        if (part1 < part2) return true;
        if (part1 > part2) return false;
    }
    return false;
}

void print_version(const std::vector<int>& version) {
    for (size_t i = 0; i < version.size(); ++i) {
        if (i > 0) std::cout << ".";
        std::cout << version[i];
    }
}