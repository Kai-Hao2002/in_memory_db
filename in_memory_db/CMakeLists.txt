
# in_memory_db/CMakeLists.txt
# 找出所有 src 下 cpp 檔案
file(GLOB_RECURSE DB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.hpp")

# 找 SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_library(in_memory_db_lib ${DB_SOURCES} ${HEADERS})
target_include_directories(in_memory_db_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Catch2/src
)

# 連結 SFML 到 lib
target_link_libraries(in_memory_db_lib PRIVATE sfml-graphics sfml-window sfml-system)

add_executable(in_memory_db main.cpp)
target_link_libraries(in_memory_db PRIVATE in_memory_db_lib)

enable_testing()
add_subdirectory(tests)
