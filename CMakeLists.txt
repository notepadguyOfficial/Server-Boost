cmake_minimum_required(VERSION 3.14)

project(Server VERSION 1.0 LANGUAGES CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set policy for CMP0167 to avoid warnings
cmake_policy(SET CMP0167 NEW)

# Disable Warnings for unused functions, variable, etc..
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-unused-variable -Wno-unused-parameter -Wno-unused-function)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/wd4100)
endif()

# --- [Git metadata]
execute_process(
    COMMAND git config --get remote.origin.url
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REPO_URL
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git log -1 --format=%cd --date=iso-strict
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Detect architecture (32-bit vs 64-bit)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_BITS "64")
else()
    set(ARCH_BITS "32")
endif()

# Detect OS and architecture type
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_NAME "Win64")
    else()
        set(ARCH_NAME "Win32")
    endif()
elseif(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(ARCH_NAME "Arm64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "amd64")
        set(ARCH_NAME "x64")
    endif()
endif()

# Derive repo name from URL (strip path and .git)
set(GIT_REPO_NAME "")
if(GIT_REPO_URL MATCHES ".*/([^/]+)\\.git$")
    set(GIT_REPO_NAME "${CMAKE_MATCH_1}")
elseif(GIT_REPO_URL MATCHES ".*/([^/]+)$")
    set(GIT_REPO_NAME "${CMAKE_MATCH_1}")
else()
    get_filename_component(GIT_REPO_NAME ${CMAKE_SOURCE_DIR} NAME)
endif()

if(WIN32)
    execute_process(
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/extract/header.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE VERSION_INFO
    )

    foreach(line IN LISTS VERSION_INFO)
        string(REPLACE "=" ";" pair ${line})
        list(GET pair 0 key)
        list(GET pair 1 val)
        set(${key} "${val}" CACHE INTERNAL "")
    endforeach()

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/version.rc.in
                   ${CMAKE_CURRENT_BINARY_DIR}/resources/version.rc @ONLY)

    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/resources/version.rc)
endif()

# Pass the values as compile definitions to your code
add_compile_definitions(
    GIT_REPO_URL="${GIT_REPO_URL}"
    GIT_REPO_NAME="${GIT_REPO_NAME}"
    GIT_COMMIT_HASH="${GIT_COMMIT_HASH}"
    GIT_BRANCH="${GIT_BRANCH}"
    GIT_COMMIT_DATE="${GIT_COMMIT_DATE}"
    ARCH_NAME="${ARCH_NAME}"
)

# Find Boost - specify required components (we only need Boost::log and Boost::system)
find_package(Boost 1.88 REQUIRED COMPONENTS system filesystem log log_setup thread)

# Find libpqxx separately
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

# Add source files
set(SOURCES
    src/main.cpp
    src/Server/Common/Version.cpp
    src/Server/Common/Helper.cpp
    src/Server/Common/Logs/Logs.cpp
    src/Server/Common/Container/Container.cpp
    src/Server/Common/Database/Connection/Connection.cpp
)

# Add headers
set(HEADERS
    src/Server/Common/Global.h
    src/Server/Common/Version.h
    src/Server/Common/Helper.h
    src/Server/Common/Logs/Logs.h
    src/Server/Common/Container/Container.h
    src/Server/Common/Database/Connection/Connection.h
    src/Server/Common/Database/Connection/ConnectionStringBuilder.h
    src/Server/Common/Database/Query/QueryBuilder.h
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
    ${LIBPQXX_LIBRARIES}
)

# Specify include directories for Boost and your project's headers
target_include_directories(Server PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${LIBPQXX_INCLUDE_DIRS}
    src
    src/Server
    src/Server/Common
    src/Server/Common/Logs
    src/Server/Common/Container
    src/Server/Common/Database
    src/Server/Common/Database/Connection
    src/Server/Common/Database/Query
)

# Enable warnings or any extra compiler flags for C++20
if(MSVC)
    target_compile_options(Server PRIVATE /W4 /permissive-)
else()
    target_compile_options(Server PRIVATE -Wall -Wextra -Wpedantic)
endif()
