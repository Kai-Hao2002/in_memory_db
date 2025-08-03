// tests/test_update.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Update.hpp"

using namespace db;
using namespace statement;

TEST_CASE("UPDATE modifies matching rows", "[update]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });

    db.insert("users", { {"id", 1}, {"name", "Alice"} });

    std::string sql = "UPDATE users SET name = 'Alicia' WHERE id = 1";
    auto stmt = Parser(sql).parse();
    auto* update_stmt = dynamic_cast<Update*>(stmt.get());
    REQUIRE(update_stmt != nullptr);

    update_stmt->execute(db);
    const auto& rows = db.get_table("users").get_rows();
    REQUIRE(std::get<std::string>(rows[0].values[1]) == "Alicia");

}

TEST_CASE("UPDATE with no match does not change data", "[update]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });

    db.insert("users", { {"id", 2}, {"name", "Bob"} });

    std::string sql = "UPDATE users SET name = 'Changed' WHERE id = 999";
    auto stmt = Parser(sql).parse();
    auto* update_stmt = dynamic_cast<Update*>(stmt.get());
    REQUIRE(update_stmt != nullptr);

    update_stmt->execute(db);
    const auto& rows = db.get_table("users").get_rows();
    REQUIRE(std::get<std::string>(rows[0].values[1]) == "Bob");

}
