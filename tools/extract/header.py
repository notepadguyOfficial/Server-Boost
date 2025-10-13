import re

defines = {}
with open("src/Server/Common/Version.h") as f:
    for line in f:
        m = re.match(r'#define\s+(\w+)\s+"?([^"\s]+)"?', line)
        if m:
            defines[m.group(1)] = m.group(2)
            
print("VERSION_MAJOR={}".format(defines.get("PROJECT_VERSION_MAJOR", "0")))
print("VERSION_MINOR={}".format(defines.get("PROJECT_VERSION_MINOR", "0")))
print("VERSION_PATCH={}".format(defines.get("PROJECT_VERSION_PATCH", "0")))
print("COMPANY_NAME={}".format(defines.get("PROJECT_COMPANY_NAME", "Unknown")))
print("PROJECT_NAME={}".format(defines.get("PROJECT_NAME", "MyApp")))