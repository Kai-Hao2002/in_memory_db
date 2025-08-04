// tests/test_persistence.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Insert.hpp"

#include <fstream>

using namespace db;
using namespace statement;

TEST_CASE("Database persistence: save and load", "[persistence]") {
    Database db;
    db.create_table("students", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });

    std::string insert_sql1 = "INSERT INTO students (id, name) VALUES (1, 'Alice')";
    std::string insert_sql2 = "INSERT INTO students (id, name) VALUES (2, 'Bob')";

    auto stmt1 = Parser(insert_sql1).parse();
    auto* insert1 = dynamic_cast<Insert*>(stmt1.get());
    REQUIRE(insert1 != nullptr);
    insert1->execute(db);

    auto stmt2 = Parser(insert_sql2).parse();
    auto* insert2 = dynamic_cast<Insert*>(stmt2.get());
    REQUIRE(insert2 != nullptr);
    insert2->execute(db);

    std::string filename = "test_db_backup.txt";
    db.save_to_file(filename);

    Database db2;
    db2.load_from_file(filename);

    REQUIRE(db2.has_table("students"));

    const Table& tbl = db2.get_table("students");
    const auto& rows = tbl.get_rows();
    REQUIRE(rows.size() == 2);

    REQUIRE(std::get<int>(rows[0].values[0]) == 1);
    REQUIRE(std::get<std::string>(rows[0].values[1]) == "Alice");

    REQUIRE(std::get<int>(rows[1].values[0]) == 2);
    REQUIRE(std::get<std::string>(rows[1].values[1]) == "Bob");

    std::remove(filename.c_str());
}
