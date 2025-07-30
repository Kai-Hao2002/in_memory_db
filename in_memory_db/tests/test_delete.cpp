// tests/test_delete.cpp
// tests/test_delete.cpp
#include <catch2/catch_test_macros.hpp>
#include "db/Table.hpp"

using namespace db;

TEST_CASE("Delete rows from table", "[delete]") {
    Table table("Users", {
        {"id", ColumnType::INT},
        {"name", ColumnType::STRING}
    });

    table.insert({{"id", 1}, {"name", "Alice"}});
    table.insert({{"id", 2}, {"name", "Bob"}});
    table.insert({{"id", 3}, {"name", "Charlie"}});

    table.delete_where("id", 2);

    auto rows = table.select({"id", "name"});
    REQUIRE(rows.size() == 2);
    REQUIRE(std::get<int>(rows[0].values[0]) == 1);
    REQUIRE(std::get<std::string>(rows[0].values[1]) == "Alice");
    REQUIRE(std::get<int>(rows[1].values[0]) == 3);
    REQUIRE(std::get<std::string>(rows[1].values[1]) == "Charlie");
}
