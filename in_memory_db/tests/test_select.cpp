// tests/test_select.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Select.hpp"
#include "statement/CreateTable.hpp"
#include "statement/Insert.hpp"

using namespace db;
using namespace statement;

TEST_CASE("Basic SELECT with WHERE condition", "[select]") {
    Database db;

    // 先建表並插入資料，這樣後面所有測試區塊都會用到
    db.create_table("users", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });
    db.insert("users", { {"id", 1}, {"name", "Alice"} });
    db.insert("users", { {"id", 2}, {"name", "Bob"} });

    SECTION("Parse and execute SELECT statement") {
        std::string sql = "SELECT id, name FROM users WHERE id = 1";
        Parser parser(sql);
        auto stmt = parser.parse();

        auto select_stmt = dynamic_cast<Select*>(stmt.get());
        REQUIRE(select_stmt != nullptr);

        select_stmt->execute(db);

        const auto& results = select_stmt->get_results();
        REQUIRE(results.size() == 1);

        REQUIRE(std::get<int>(results[0][0]) == 1);
        REQUIRE(std::get<std::string>(results[0][1]) == "Alice");

    }
}


TEST_CASE("SELECT with no match returns empty", "[select]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });

    db.insert("users", { {"id", 1}, {"name", "Alice"} });

    std::string sql = "SELECT id, name FROM users WHERE id = 999";
    auto stmt = Parser(sql).parse();
    auto select_stmt = dynamic_cast<Select*>(stmt.get());
    REQUIRE(select_stmt != nullptr);

    select_stmt->execute(db);
    REQUIRE(select_stmt->get_results().empty());
}

TEST_CASE("SELECT * works correctly", "[select][select_all]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT},
        {"name", ColumnType::STRING}
    });

    db.insert("users", { {"id", 3}, {"name", "Charlie"} });

    std::string sql = "SELECT * FROM users WHERE name = 'Charlie'";
    auto stmt = Parser(sql).parse();
    auto select_stmt = dynamic_cast<Select*>(stmt.get());
    REQUIRE(select_stmt != nullptr);

    select_stmt->execute(db);
    const auto& results = select_stmt->get_results();
    REQUIRE(results.size() == 1);
    REQUIRE(std::get<int>(results[0][0]) == 3);
    REQUIRE(std::get<std::string>(results[0][1]) == "Charlie");
}
