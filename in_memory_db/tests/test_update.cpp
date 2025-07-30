// tests/test_update.cpp
#include <catch2/catch_test_macros.hpp>
#include "db/Database.hpp"

using namespace db;

TEST_CASE("Update rows based on condition", "[update]") {
    Table table("Tasks", {
        {"id", ColumnType::INT},
        {"status", ColumnType::STRING}
    });

    table.insert({{"id", 1}, {"status", "open"}});
    table.insert({{"id", 2}, {"status", "open"}});

    table.update_where("status", Value("open"), "status", Value("closed"));

    auto rows = table.select({"id", "status"});
    REQUIRE(rows.size() == 2);
    REQUIRE(std::get<std::string>(rows[0].values[1]) == "closed");
    REQUIRE(std::get<std::string>(rows[1].values[1]) == "closed");
}
