cmake_minimum_required(VERSION 3.14)

project(Server VERSION 1.0 LANGUAGES CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set policy for CMP0167 to avoid warnings
cmake_policy(SET CMP0167 NEW)

# Find Boost - specify required components (we only need Boost::log and Boost::system)
find_package(Boost 1.88 REQUIRED COMPONENTS system filesystem log log_setup thread)

# Add source files
set(SOURCES
    src/main.cpp
    src/Server/Common/Logs/Logs.cpp
    src/Server/Common/Container/Container.cpp
)

# Add headers
set(HEADERS
    src/Server/Common/Logs/Logs.h
    src/Server/Common/Container/Container.h
)

# Create the executable
add_executable(Server ${SOURCES} ${HEADERS})

# Define BOOST_LOG_DYN_LINK for dynamic linking (optional)
target_compile_definitions(Server PRIVATE BOOST_LOG_DYN_LINK)

# Link specific Boost components to the target
target_link_libraries(Server
    Boost::log
    Boost::log_setup
    Boost::system
    Boost::filesystem
    Boost::thread
)

# Specify include directories for Boost and your project's headers
target_include_directories(Server PRIVATE
    ${Boost_INCLUDE_DIRS}
    src
    src/Server/Common/Logs
    src/Server/Common/Container
)

# Enable warnings or any extra compiler flags for C++20
if(MSVC)
    target_compile_options(Server PRIVATE /W4 /permissive-)
else()
    target_compile_options(Server PRIVATE -Wall -Wextra -Wpedantic)
endif()
