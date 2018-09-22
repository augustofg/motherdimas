cmake_minimum_required(VERSION 3.1)
project(motherdimas)

# This project requires C++11
set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Docopt library
add_subdirectory(third_party/docopt.cpp docopt_build)

set(motherdimas_src_files)

# Display source files
list(APPEND motherdimas_src_files src/display/display.cpp)

# Input source files
list(APPEND motherdimas_src_files src/input/input.cpp)

# Interactions source files
list(APPEND motherdimas_src_files src/interactions/interactions.cpp)

# Log and debug messages source files
list(APPEND motherdimas_src_files src/utils/msglog.cpp)

add_executable(motherdimas src/main.cpp ${motherdimas_src_files})
target_link_libraries(motherdimas ${SLD2_LIBRARIES} docopt_s)
