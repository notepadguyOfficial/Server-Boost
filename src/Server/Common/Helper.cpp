#include "Helper.h"

/**
 * @brief Parse a dot-separated version string into its numeric components.
 *
 * Splits `version` on '.' and converts each segment to an integer. Empty or
 * non-numeric segments are converted to 0.
 *
 * @param version Dot-separated version string (e.g., "1.2.3").
 * @return std::vector<int> Vector of integer components in order; empty if `version` is empty.
 */
std::vector<int> parse(const std::string& version) {
    std::vector<int> parts;
    std::istringstream iss(version);
    std::string token;
    while (std::getline(iss, token, '.')) {
        parts.push_back(std::atoi(token.c_str()));
    }
    return parts;
}

/**
 * @brief Determines whether version vector `v1` is less than `v2` in component-wise lexicographic order.
 *
 * Missing components are treated as 0 for the purposes of comparison.
 *
 * @param v1 First version represented as a vector of integer components.
 * @param v2 Second version represented as a vector of integer components.
 * @return `true` if `v1` is less than `v2` when compared component by component (treating missing components as 0), `false` otherwise.
 */
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

/**
 * @brief Writes a version vector as dot-separated components to standard output.
 *
 * Prints the integers from `version` separated by '.' to std::cout without adding a trailing newline.
 *
 * @param version Sequence of integer version components (for example, {1, 2, 3} -> "1.2.3").
 */
void print_version(const std::vector<int>& version) {
    for (size_t i = 0; i < version.size(); ++i) {
        if (i > 0) std::cout << ".";
        std::cout << version[i];
    }
}