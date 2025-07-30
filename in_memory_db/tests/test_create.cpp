// tests/test_create.cpp
#include "db/Table.hpp"
#include "db/Row.hpp"
#include "db/Column.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace db;

TEST_CASE("Table creation and insertion") {
    // Step 1: 建立表格
    std::vector<Column> columns = {
        {"id", ColumnType::INT},
        {"name", ColumnType::STRING}
    };
    Table table("users", columns);

    // Step 2: 插入第一筆資料
    table.insert({
        {"id", 1},
        {"name", std::string("Alice")}
    });

    // Step 3: 插入第二筆資料
    table.insert({
        {"id", 2},
        {"name", std::string("Bob")}
    });

    // Step 4: 全部查詢
    auto all_rows = table.select({"id", "name"});

    REQUIRE(all_rows.size() == 2);
    REQUIRE(std::get<int>(all_rows[0].values[0]) == 1);             // id at index 0
    REQUIRE(std::get<std::string>(all_rows[0].values[1]) == "Alice"); // name at index 1
    REQUIRE(std::get<int>(all_rows[1].values[0]) == 2);
    REQUIRE(std::get<std::string>(all_rows[1].values[1]) == "Bob");
}
