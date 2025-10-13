#pragma once

// Win32 - Win64 Porgram Metadata
#define PROJECT_VERSION_MAJOR 1
#define PROJECT_VERSION_MINOR 0
#define PROJECT_VERSION_PATCH 1

#define PROJECT_COMPANY_NAME "HydroHub Corp."
#define PROJECT_NAME "HydroHub"

// helper
#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

// Embedding Extern Constant
extern const char* APP_NAME;
extern const char* APP_COMPANY_NAME;
extern const char* APP_VERSION;
