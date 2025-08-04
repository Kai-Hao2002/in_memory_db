#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/SelectJoin.hpp"

using namespace db;
using namespace statement;

TEST_CASE("INNER JOIN returns correct rows", "[select][join]") {
    Database db;

    db.create_table("students", {
        {"id", ColumnType::INT}, {"name", ColumnType::STRING}
    });

    db.create_table("scores", {
        {"s_id", ColumnType::INT}, {"score", ColumnType::INT}
    });

    Parser("INSERT INTO students(id, name) VALUES (1, 'Alice')").parse()->execute(db);
    Parser("INSERT INTO students(id, name) VALUES (2, 'Bob')").parse()->execute(db);

    Parser("INSERT INTO scores(s_id, score) VALUES (1, 95)").parse()->execute(db);
    Parser("INSERT INTO scores(s_id, score) VALUES (2, 88)").parse()->execute(db);
    Parser("INSERT INTO scores(s_id, score) VALUES (3, 76)").parse()->execute(db); 

    std::string sql = "SELECT students.name, scores.score FROM students INNER JOIN scores ON students.id = scores.s_id";
    auto stmt = Parser(sql).parse();
    auto* select_join_stmt = dynamic_cast<SelectJoin*>(stmt.get());
    REQUIRE(select_join_stmt != nullptr);

    select_join_stmt->execute(db);

    const auto& results = select_join_stmt->get_results();
    const auto& headers = select_join_stmt->get_output_column_names();

    REQUIRE(headers.size() == 2);
    REQUIRE(headers[0] == "students.name");
    REQUIRE(headers[1] == "scores.score");

    REQUIRE(results.size() == 2); 

    REQUIRE(std::get<std::string>(results[0][0]) == "Alice");
    REQUIRE(std::get<int>(results[0][1]) == 95);

    REQUIRE(std::get<std::string>(results[1][0]) == "Bob");
    REQUIRE(std::get<int>(results[1][1]) == 88);
}
