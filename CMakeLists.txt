cmake_minimum_required(VERSION 3.15)
project(in_memory_db VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 指定 include 目錄
include_directories(${PROJECT_SOURCE_DIR}/include)

# 加入子目錄
add_subdirectory(in_memory_db)
enable_testing()

file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR})