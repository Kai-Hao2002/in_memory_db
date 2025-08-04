// tests/test_select_where.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Select.hpp"

using namespace db;
using namespace statement;

TEST_CASE("SELECT with WHERE conditions", "[select][where]") {
    Database db;
    db.create_table("users", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });

    Parser("INSERT INTO users (id, name) VALUES (1, 'Alice')").parse()->execute(db);
    Parser("INSERT INTO users (id, name) VALUES (2, 'Bob')").parse()->execute(db);
    Parser("INSERT INTO users (id, name) VALUES (3, 'Charlie')").parse()->execute(db);
    Parser("INSERT INTO users (id, name) VALUES (4, 'Diana')").parse()->execute(db);

    SECTION("WHERE name = 'Bob'") {
        auto stmt = Parser("SELECT id, name FROM users WHERE name = 'Bob'").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        REQUIRE(select != nullptr);
        select->execute(db);

        const auto& results = select->get_results();
        REQUIRE(results.size() == 1);
        REQUIRE(std::get<int>(results[0][0]) == 2);
    }

    SECTION("WHERE id != 1") {
        auto stmt = Parser("SELECT id FROM users WHERE id != 1").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        REQUIRE(select != nullptr);
        select->execute(db);

        const auto& results = select->get_results();
        REQUIRE(results.size() == 3);
    }

    SECTION("WHERE id < 3") {
        auto stmt = Parser("SELECT name FROM users WHERE id < 3").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        REQUIRE(select->get_results().size() == 2);
        REQUIRE(std::get<std::string>(select->get_results()[0][0]) == "Alice");
    }

    SECTION("WHERE id <= 2") {
        auto stmt = Parser("SELECT id FROM users WHERE id <= 2").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        REQUIRE(select->get_results().size() == 2);
    }

    SECTION("WHERE id > 2") {
        auto stmt = Parser("SELECT name FROM users WHERE id > 2").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        const auto& res = select->get_results();
        REQUIRE(res.size() == 2);
        REQUIRE(std::get<std::string>(res[0][0]) == "Charlie");
        REQUIRE(std::get<std::string>(res[1][0]) == "Diana");
    }

    SECTION("WHERE id >= 3") {
        auto stmt = Parser("SELECT id FROM users WHERE id >= 3").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        REQUIRE(select->get_results().size() == 2);
    }

    SECTION("WHERE name = 'Alice' AND id = 1") {
        auto stmt = Parser("SELECT id FROM users WHERE name = 'Alice' AND id = 1").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        const auto& res = select->get_results();
        REQUIRE(res.size() == 1);
        REQUIRE(std::get<int>(res[0][0]) == 1);
    }

    SECTION("WHERE name = 'Alice' AND id = 2 (no match)") {
        auto stmt = Parser("SELECT id FROM users WHERE name = 'Alice' AND id = 2").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        REQUIRE(select->get_results().empty());
    }

    SECTION("WHERE name = 'Alice' OR name = 'Bob'") {
        auto stmt = Parser("SELECT name FROM users WHERE name = 'Alice' OR name = 'Bob'").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        const auto& res = select->get_results();
        REQUIRE(res.size() == 2);
        REQUIRE(std::get<std::string>(res[0][0]) == "Alice");
        REQUIRE(std::get<std::string>(res[1][0]) == "Bob");
    }

    SECTION("WHERE (name = 'Alice' OR name = 'Bob') AND id = 2") {
        auto stmt = Parser("SELECT name FROM users WHERE (name = 'Alice' OR name = 'Bob') AND id = 2").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        const auto& res = select->get_results();
        REQUIRE(res.size() == 1);
        REQUIRE(std::get<std::string>(res[0][0]) == "Bob");
    }

    SECTION("WHERE id = 999 (no match)") {
        auto stmt = Parser("SELECT name FROM users WHERE id = 999").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        select->execute(db);

        REQUIRE(select->get_results().empty());
    }
}
