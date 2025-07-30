# in_memory_db/tests/CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.3
)
FetchContent_MakeAvailable(Catch2)

# 取得當前目錄所有 cpp 檔案做為測試檔案
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(in_memory_db_tests ${TEST_SOURCES})

# 連結資料庫函式庫和 Catch2 測試主函式
target_link_libraries(in_memory_db_tests PRIVATE in_memory_db_lib Catch2::Catch2WithMain)

# 設定 include path（改成專案中絕對路徑）
target_include_directories(in_memory_db_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# 啟用 CTest 和 Catch2 的自動測試發現
include(CTest)
include(Catch)

catch_discover_tests(in_memory_db_tests)
