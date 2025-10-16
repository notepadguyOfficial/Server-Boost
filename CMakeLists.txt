cmake_minimum_required(VERSION 3.14)

project(Server VERSION 1.0 LANGUAGES CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set policy for CMP0167 to avoid warnings
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

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
# Configure to use available Boost Library Version installed at least 1.83 or above
find_package(Boost 1.83 REQUIRED COMPONENTS system filesystem log log_setup thread)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found.")
endif()

message(STATUS "Using Boost version ${Boost_VERSION} (>= 1.83)")

# Find libpqxx separately
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

# Add source files
set(SOURCES
    src/Common/Helper.cpp
    src/Common/Logs/Logs.cpp
    src/Common/Version.cpp
    src/Server/Api/Context/Context.cpp
    src/Server/Api/Handle/Handle.cpp
    src/Server/Api/Models/Customer/Customer.cpp
    src/Server/Api/Routes/Routes.cpp
    src/Server/Console/Console.cpp
    src/Server/Container/Container.cpp
    src/Server/Database/Connection/Connection.cpp
    src/Server/Http/Http.cpp
    src/Server/Server.cpp
    src/main.cpp
)

# Add headers
set(HEADERS
    src/Common/Global.h
    src/Common/Helper.h
    src/Common/Logs/Logs.h
    src/Common/Logs/StreamBuffer.h
    src/Common/Logs/TextExtra.h
    src/Common/Version.h
    src/Server/Api/Context/Context.h
    src/Server/Api/Handle/Handle.h
    src/Server/Api/Models/Customer/Customer.h
    src/Server/Api/Routes/Routes.h
    src/Server/Console/Console.h
    src/Server/Container/Container.h
    src/Server/Database/Connection/Connection.h
    src/Server/Database/Connection/ConnectionStringBuilder.h
    src/Server/Database/Query/QueryBuilder.h
    src/Server/Http/Http.h
    src/Server/Server.h
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
    src/Common
    src/Server
    src/Common/Logs
    src/Server/Api
    src/Server/Console
    src/Server/Container
    src/Server/Database
    src/Server/Http
    src/Server/Api/Context
    src/Server/Api/Handle
    src/Server/Api/Models
    src/Server/Api/Routes
    src/Server/Api/Serializers
    src/Server/Database/Connection
    src/Server/Database/Query
    src/Server/Api/Handle/Admin
    src/Server/Api/Handle/Customer
    src/Server/Api/Handle/Owner
    src/Server/Api/Handle/Staff
    src/Server/Api/Models/Customer
)

# Enable warnings or any extra compiler flags for C++20
if(MSVC)
    target_compile_options(Server PRIVATE /W4 /permissive-)
else()
    target_compile_options(Server PRIVATE -Wall -Wextra -Wpedantic)
endif()
