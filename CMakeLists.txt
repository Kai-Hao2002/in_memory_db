cmake_minimum_required(VERSION 3.15)
project(in_memory_db VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# ===============================
# Detect platform and set generator
# ===============================
if(NOT CMAKE_GENERATOR)
    if(APPLE)
        message(STATUS "macOS detected: using Xcode generator")
        set(CMAKE_GENERATOR "Xcode" CACHE STRING "" FORCE)
    elseif(WIN32)
        message(STATUS "Windows detected: using Ninja generator")
        set(CMAKE_GENERATOR "Ninja" CACHE STRING "" FORCE)
    else()
        message(STATUS "Linux detected: using Ninja generator")
        set(CMAKE_GENERATOR "Ninja" CACHE STRING "" FORCE)
    endif()
endif()
# Specify include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(in_memory_db)
enable_testing()

file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR})