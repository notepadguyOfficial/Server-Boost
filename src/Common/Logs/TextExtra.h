#pragma once
#include <array>
#include <string_view>
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>

#define SYMBOL_CHECK_PRINT "[\033[32m✓\033[0m]"
#define SYMBOL_WRONG_PRINT "[\033[31mX\033[0m]"

#define COLOR_GRAY "\033[90m"
#define COLOR_BLUE "\033[34m"
#define COLOR_CYAN "\033[36m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BRIGHT_YELLOW "\033[93m"
#define COLOR_RED "\033[31m"
#define COLOR_BRIGHT_RED "\033[1;31;40m"
#define COLOR_RESET "\033[0m"

enum class Protocol : uint8_t
{
    None = 0x00,
    Http = 0x01,
    Websocket = 0x02,
    Database = 0x03,
    Settings = 0x04,
    Size = 0x05
};

constexpr std::array<std::string_view, static_cast<size_t>(Protocol::Size)> PROTOCOL_MAP = {
    "NONE", "HTTP", "WEBSOCKET", "DATABASE", "SETTINGS"};

constexpr std::array<std::string_view, 6> SEVERITY_MAP = {
    "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL"};

constexpr std::array<const char *, 6> SEVERITY_COLOR_MAP = {
    COLOR_GRAY, COLOR_BLUE, COLOR_GREEN, COLOR_YELLOW, COLOR_RED, COLOR_BRIGHT_RED};

inline constexpr std::string_view get_protocol_name(Protocol protocol)
{
    auto index = static_cast<size_t>(protocol);
    return index < PROTOCOL_MAP.size() ? PROTOCOL_MAP[index] : "UNKNOWN";
}

inline constexpr std::string_view get_severity_name(boost::log::trivial::severity_level level)
{
    return static_cast<size_t>(level) < SEVERITY_MAP.size() ? SEVERITY_MAP[level] : "UNKNOWN";
}

inline constexpr const char *get_severity_color(boost::log::trivial::severity_level level)
{
    return static_cast<size_t>(level) < SEVERITY_COLOR_MAP.size() ? SEVERITY_COLOR_MAP[level] : "";
}

inline std::string trim(const std::string &path)
{
    const std::string base = "src";
    boost::filesystem::path p(path);
    std::string temp = p.generic_string();

    auto position = temp.find(base + "/");
    if (position != std::string::npos)
        return temp.substr(position);
    return p.filename().string();
}
