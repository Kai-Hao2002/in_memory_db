// tests/test_insert.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Insert.hpp"

using namespace db;
using namespace statement;

TEST_CASE("INSERT adds row to table", "[insert]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });

    std::string sql = "INSERT INTO users (id, name) VALUES (1, 'Alice')";
    auto stmt = Parser(sql).parse();
    auto* insert_stmt = dynamic_cast<Insert*>(stmt.get());
    REQUIRE(insert_stmt != nullptr);

    insert_stmt->execute(db);

    const Table& table = db.get_table("users");
    const auto& rows = table.get_rows();
    REQUIRE(rows.size() == 1);
    REQUIRE(std::get<int>(rows[0].values[0]) == 1);
    REQUIRE(std::get<std::string>(rows[0].values[1]) == "Alice");
}


TEST_CASE("INSERT fails with wrong type", "[insert][error]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT}
    });

    std::string sql = "INSERT INTO users (id) VALUES ('wrong')";
    auto stmt = Parser(sql).parse();

    auto* insert_stmt = dynamic_cast<Insert*>(stmt.get());
    REQUIRE(insert_stmt != nullptr);

    REQUIRE_THROWS_WITH(insert_stmt->execute(db), Catch::Matchers::ContainsSubstring("Type mismatch"));

}
