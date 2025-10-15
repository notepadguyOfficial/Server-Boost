import os
import re
from pathlib import Path

SOURCE_DIR = 'src'
CMAKE_FILE = 'CMakeLists.txt'

def find_files_and_dirs(base_dir):
    cpp_files = []
    header_files = set()
    include_dirs = set()

    for root, dirs, files in os.walk(base_dir):
        rel_root = root.replace('\\', '/')
        include_dirs.add(rel_root)

        for file in files:
            rel_path = os.path.join(root, file).replace('\\', '/')
            if file.endswith('.cpp'):
                cpp_files.append(rel_path)
            elif file.endswith('.h') or file.endswith('.hpp'):
                header_files.add(rel_path)

    return sorted(cpp_files), sorted(header_files), sorted(include_dirs)

def update_cmake_lists(cmake_path, cpp_files, header_files, include_dirs):
    with open(cmake_path, 'r') as f:
        content = f.read()

    def format_block(name, items):
        return f"set({name}\n    " + '\n    '.join(items) + '\n)'

    def format_includes(dirs):
        sorted_dirs = sorted(dirs, key=lambda d: (d.count('/'), d))
        return "target_include_directories(Server PRIVATE\n    ${Boost_INCLUDE_DIRS}\n    ${LIBPQXX_INCLUDE_DIRS}\n    " + '\n    '.join(sorted_dirs) + '\n)'

    content = re.sub(
        r'set\(SOURCES.*?\)',
        format_block('SOURCES', cpp_files),
        content,
        flags=re.DOTALL
    )

    content = re.sub(
        r'set\(HEADERS.*?\)',
        format_block('HEADERS', header_files),
        content,
        flags=re.DOTALL
    )

    content = re.sub(
        r'target_include_directories\(Server PRIVATE.*?\)',
        format_includes(include_dirs),
        content,
        flags=re.DOTALL
    )

    with open(cmake_path, 'w') as f:
        f.write(content)

    print(f"Updated {cmake_path} with {len(cpp_files)} sources, {len(header_files)} headers, and {len(include_dirs)} include directories.")

if __name__ == "__main__":
    if not os.path.exists(CMAKE_FILE):
        print(f"{CMAKE_FILE} not found in current directory.")
    else:
        cpp_files, header_files, include_dirs = find_files_and_dirs(SOURCE_DIR)
        update_cmake_lists(CMAKE_FILE, cpp_files, header_files, include_dirs)
