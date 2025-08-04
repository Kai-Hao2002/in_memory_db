// tests/test_data_types.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Select.hpp"

using namespace db;
using namespace statement;
using Catch::Approx;


TEST_CASE("Supports all basic data types", "[types]") {
    Database db;

    // Create a table containing all types
    Parser("CREATE TABLE test_types(id INT, name STRING, score FLOAT, accuracy DOUBLE, passed BOOL)").parse()->execute(db);

    Parser("INSERT INTO test_types(id, name, score, accuracy, passed) VALUES (1, 'Alice', 95.5, 99.999, true)").parse()->execute(db);
    Parser("INSERT INTO test_types(id, name, score, accuracy, passed) VALUES (2, 'Bob', 87.25, 88.12345, false)").parse()->execute(db);

    SECTION("SELECT all columns") {
        auto stmt = Parser("SELECT id, name, score, accuracy, passed FROM test_types").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        REQUIRE(select != nullptr);
        select->execute(db);

        const auto& results = select->get_results();
        REQUIRE(results.size() == 2);

        REQUIRE(std::get<int>(results[0][0]) == 1);
        REQUIRE(std::get<std::string>(results[0][1]) == "Alice");
        if (std::holds_alternative<float>(results[0][2])) {
            REQUIRE(std::get<float>(results[0][2]) == Catch::Approx(95.5f));
        } else if (std::holds_alternative<double>(results[0][2])) {
            REQUIRE(std::get<double>(results[0][2]) == Catch::Approx(95.5));
        } else {
            FAIL("Unexpected type for 'score' column");
        }
        if (std::holds_alternative<float>(results[0][3])) {
            REQUIRE(std::get<float>(results[0][3]) == Catch::Approx(99.999));
        } else if (std::holds_alternative<double>(results[0][3])) {
            REQUIRE(std::get<double>(results[0][3]) == Catch::Approx(99.999));
        } else {
            FAIL("Unexpected type for 'score' column");
        }
        REQUIRE(std::get<bool>(results[0][4]) == true);

        REQUIRE(std::get<int>(results[1][0]) == 2);
        REQUIRE(std::get<std::string>(results[1][1]) == "Bob");
        if (std::holds_alternative<float>(results[1][2])) {
            REQUIRE(std::get<float>(results[1][2]) == Catch::Approx(87.25f));
        } else if (std::holds_alternative<double>(results[1][2])) {
            REQUIRE(std::get<double>(results[1][2]) == Catch::Approx(87.25f));
        } else {
            FAIL("Unexpected type for 'score' column");
        }
        if (std::holds_alternative<float>(results[1][3])) {
            REQUIRE(std::get<float>(results[1][3]) == Catch::Approx(88.12345));
        } else if (std::holds_alternative<double>(results[1][3])) {
            REQUIRE(std::get<double>(results[1][3]) == Catch::Approx(88.12345));
        } else {
            FAIL("Unexpected type for 'score' column");
        }
        REQUIRE(std::get<bool>(results[1][4]) == false);
    }

    SECTION("WHERE passed = true") {
        auto stmt = Parser("SELECT name FROM test_types WHERE passed = true").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        REQUIRE(select != nullptr);
        select->execute(db);

        const auto& results = select->get_results();
        REQUIRE(results.size() == 1);
        REQUIRE(std::get<std::string>(results[0][0]) == "Alice");
    }

    SECTION("Persistence (save + load)") {
        const std::string filename = "test_types.txt";
        REQUIRE_NOTHROW(db.save_to_file(filename));

        Database db2;
        REQUIRE_NOTHROW(db2.load_from_file(filename));

        auto stmt = Parser("SELECT id, name, score, accuracy, passed FROM test_types").parse();
        auto* select = dynamic_cast<Select*>(stmt.get());
        REQUIRE(select != nullptr);
        select->execute(db2);

        const auto& results = select->get_results();
        REQUIRE(results.size() == 2);

        REQUIRE(std::get<int>(results[1][0]) == 2);
        REQUIRE(std::get<std::string>(results[1][1]) == "Bob");

        std::remove(filename.c_str());  
    }
}
