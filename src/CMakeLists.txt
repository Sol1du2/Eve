cmake_minimum_required(VERSION 3.19)

project(Eve VERSION 1.0)

configure_file(Config.h.in Config.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost 1.73.0 REQUIRED COMPONENTS system thread)
find_package(nlohmann_json 3.2.0 REQUIRED)
set(JSON_INCLUDE_DIR "/usr/local/include/nlohmann")

add_subdirectory(eve)
