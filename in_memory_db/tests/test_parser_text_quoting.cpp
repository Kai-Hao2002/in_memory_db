#include <catch2/catch_all.hpp>
#include "parser/Parser.hpp"
#include "db/Database.hpp"
#include "statement/Insert.hpp"

TEST_CASE("Parser handles quoted strings correctly", "[parser][string]") {
    db::Database db;
    db.create_table("messages", {
        {"id", db::ColumnType::INT},
        {"content", db::ColumnType::STRING}
    });

    SECTION("String with semicolon inside quotes") {
        std::string sql = "INSERT INTO messages (id, content) VALUES (1, 'Hello; this is a test')";
        auto stmt = Parser(sql).parse();
        auto* insert_stmt = dynamic_cast<statement::Insert*>(stmt.get());
        REQUIRE(insert_stmt != nullptr);

        insert_stmt->execute(db);

        const auto& rows = db.get_table("messages").get_rows();
        REQUIRE(rows.size() == 1);
        REQUIRE(std::get<std::string>(rows[0].values[1]) == "Hello; this is a test");
    }

    SECTION("String containing SQL keywords inside quotes") {
        std::string sql = "INSERT INTO messages (id, content) VALUES (2, 'SELECT * FROM something')";
        auto stmt = Parser(sql).parse();
        auto* insert_stmt = dynamic_cast<statement::Insert*>(stmt.get());
        REQUIRE(insert_stmt != nullptr);

        insert_stmt->execute(db);

        const auto& rows = db.get_table("messages").get_rows();
        REQUIRE(rows.size() == 1);
        REQUIRE(std::get<std::string>(rows[0].values[1]) == "SELECT * FROM something");
    }

    SECTION("String with multiple spaces and punctuation") {
        std::string sql = "INSERT INTO messages (id, content) VALUES (3, 'Hello,    world!!!')";
        auto stmt = Parser(sql).parse();
        auto* insert_stmt = dynamic_cast<statement::Insert*>(stmt.get());
        REQUIRE(insert_stmt != nullptr);

        insert_stmt->execute(db);

        const auto& rows = db.get_table("messages").get_rows();
        REQUIRE(rows.size() == 1);
        REQUIRE(std::get<std::string>(rows[0].values[1]) == "Hello,    world!!!");
    }

    SECTION("Unterminated string literal throws error") {
        std::string sql = "INSERT INTO messages (id, content) VALUES (4, 'Missing end";
        REQUIRE_THROWS_WITH(Parser(sql).parse(), Catch::Matchers::ContainsSubstring("Unterminated string literal"));
    }

    SECTION("Double quote string if supported") {
        std::string sql = "INSERT INTO messages (id, content) VALUES (5, \"Hello with double quote\")";
        try {
            auto stmt = Parser(sql).parse();
            auto* insert_stmt = dynamic_cast<statement::Insert*>(stmt.get());
            REQUIRE(insert_stmt != nullptr);

            insert_stmt->execute(db);
            const auto& rows = db.get_table("messages").get_rows();
            REQUIRE(rows.size() == 1);
            REQUIRE(std::get<std::string>(rows.back().values[1]) == "Hello with double quote");
        } catch (const std::exception& e) {
            WARN("Double quote not yet supported: " << e.what());
        }
    }
}
