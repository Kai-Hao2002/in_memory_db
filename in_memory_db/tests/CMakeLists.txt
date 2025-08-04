# in_memory_db/tests/CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.3
)
FetchContent_MakeAvailable(Catch2)

# Get all cpp files in the current directory as test files
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(in_memory_db_tests ${TEST_SOURCES})

# Link the database library and Catch2 test main function
target_link_libraries(in_memory_db_tests PRIVATE in_memory_db_lib Catch2::Catch2WithMain)

# Set include path (change to the absolute path in the project)
target_include_directories(in_memory_db_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Enable automatic test discovery for CTest and Catch2
include(CTest)
include(Catch)

catch_discover_tests(in_memory_db_tests)
