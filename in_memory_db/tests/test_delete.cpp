// tests/test_delete.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Delete.hpp"

using namespace db;
using namespace statement;

TEST_CASE("DELETE removes matching rows", "[delete]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });

    db.insert("users", { {"id", 1}, {"name", "Alice"} });
    db.insert("users", { {"id", 2}, {"name", "Bob"} });

    std::string sql = "DELETE FROM users WHERE name = 'Alice'";
    auto stmt = Parser(sql).parse();
    auto* delete_stmt = dynamic_cast<Delete*>(stmt.get());
    REQUIRE(delete_stmt != nullptr);

    delete_stmt->execute(db);

    const auto& table = db.get_table("users");  
    const auto& rows = table.get_rows();        
    REQUIRE(rows.size() == 1);
    
    const auto& remaining_row = rows[0].values;
    REQUIRE(std::get<std::string>(remaining_row[1]) == "Bob");

}

TEST_CASE("DELETE with no match does nothing", "[delete]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT}
    });

    db.insert("users", { {"id", 1} });

    std::string sql = "DELETE FROM users WHERE id = 999";
    auto stmt = Parser(sql).parse();
    auto* delete_stmt = dynamic_cast<Delete*>(stmt.get());
    REQUIRE(delete_stmt != nullptr);

    delete_stmt->execute(db);

    const auto& table = db.get_table("users");
    REQUIRE(table.get_rows().size() == 1);
}
