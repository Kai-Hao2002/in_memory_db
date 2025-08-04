// tests/test_create.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "parser/Parser.hpp"
#include "db/Database.hpp"
#include "statement/Statement.hpp"
#include "statement/CreateTable.hpp"

using namespace db;
using namespace statement;
using namespace std::string_literals;  

TEST_CASE("CREATE TABLE statement creates a new table", "[create]") {
    Database db;

    std::string sql = "CREATE TABLE users (id INT, name STRING)";
    Parser parser(sql);
    std::unique_ptr<Statement> stmt = parser.parse();

    auto* create_stmt = dynamic_cast<CreateTableStatement*>(stmt.get());
    REQUIRE(create_stmt != nullptr);

    create_stmt->execute(db);

    auto& users_table = db.get_table("users");
    std::vector<std::string> expected_columns = { "id", "name" };
    REQUIRE(users_table.get_column_names() == expected_columns);
}

TEST_CASE("CREATE TABLE throws if table already exists", "[create][error]") {
    Database db;
    db.create_table("users", {
        Column("id", ColumnType::INT),
        Column("name", ColumnType::STRING)
    });

    std::string sql = "CREATE TABLE users (id INT, name STRING)";
    Parser parser(sql);
    std::unique_ptr<Statement> stmt = parser.parse();

    auto* create_stmt = dynamic_cast<CreateTableStatement*>(stmt.get());
    REQUIRE(create_stmt != nullptr);

    try {
        create_stmt->execute(db);
        FAIL("Expected exception not thrown");
    } catch (const std::runtime_error& e) {
        REQUIRE(std::string(e.what()).find("already exists") != std::string::npos);
    }
}
