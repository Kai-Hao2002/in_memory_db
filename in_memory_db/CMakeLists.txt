
# 找出所有 src 下 cpp 檔案
file(GLOB_RECURSE DB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.hpp")

add_library(in_memory_db_lib ${DB_SOURCES} ${HEADERS})
target_include_directories(in_memory_db_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(in_memory_db main.cpp)
target_link_libraries(in_memory_db PRIVATE in_memory_db_lib)

# 加入測試子目錄
enable_testing()
add_subdirectory(tests)
