// tests/test_insert.cpp
#include <catch2/catch_test_macros.hpp>
#include "db/Database.hpp"

using namespace db;

TEST_CASE("Insert rows into table", "[insert]") {
    Table table("Products", {
        {"id", ColumnType::INT},
        {"name", ColumnType::STRING}
    });

    table.insert({{"id", 101}, {"name", "Book"}});
    table.insert({{"id", 102}});  // Missing name, should be default ""

    auto rows = table.select({"id", "name"});
    REQUIRE(rows.size() == 2);
    REQUIRE(std::get<int>(rows[0].values[0]) == 101);
    REQUIRE(std::get<std::string>(rows[0].values[1]) == "Book");
    REQUIRE(std::get<int>(rows[1].values[0]) == 102);
    REQUIRE(std::get<std::string>(rows[1].values[1]) == "");
}
