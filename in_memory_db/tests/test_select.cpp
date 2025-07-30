// tests/test_select.cpp
#include <catch2/catch_test_macros.hpp>
#include "db/Database.hpp"

using namespace db;

TEST_CASE("Select rows with and without WHERE clause", "[select]") {
    Table table("Employees", {
        {"id", ColumnType::INT},
        {"dept", ColumnType::STRING}
    });

    table.insert({{"id", 1}, {"dept", "HR"}});
    table.insert({{"id", 2}, {"dept", "Engineering"}});
    table.insert({{"id", 3}, {"dept", "HR"}});

    SECTION("Select all rows") {
        auto rows = table.select({"id", "dept"});
        REQUIRE(rows.size() == 3);
    }

    SECTION("Select with WHERE clause") {
        auto rows = table.select({"id"}, std::make_pair("dept", Value("HR")));
        REQUIRE(rows.size() == 2);
        REQUIRE(std::get<int>(rows[0].values[0]) == 1);
        REQUIRE(std::get<int>(rows[1].values[0]) == 3);
    }
}
