//main.cpp
#include <iostream>
#include <string>
#include <memory>
#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Statement.hpp"
#include "util/Formatter.hpp"

int main() {
    db::Database database;
    bool output_csv = false; // 預設輸出 ASCII 表格

    std::cout << "Welcome to the In-Memory DB.  Enter SQL commands \n(1.type 'exit;' to quit.\n 2.type 'set output csv;' to transfer csv.\n 3.type 'set output ascii;' to transfer ascii):" << std::endl;

    std::string line;
    std::string sql;

    while (true) {
        std::cout << "SQL> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line == "SET OUTPUT CSV;" || line == "set output csv;") {
            output_csv = true;
            std::cout << "Output format set to CSV." << std::endl;
            continue;
        }

        if (line == "SET OUTPUT ASCII;" || line == "set output ascii;") {
            output_csv = false;
            std::cout << "Output format set to ASCII table." << std::endl;
            continue;
        }

        if (line == "exit;" || line == "exit") {
            break;
        }

        sql += line;
        if (!sql.empty() && sql.back() == ';') {
            sql.pop_back();

            try {
                Parser parser(sql);
                statement::StatementPtr stmt = parser.parse();

                stmt->execute(database);

                if (stmt->has_results()) {
                    // 只有 SELECT 等查詢有結果
                    auto headers = stmt->get_columns();
                    auto rows = stmt->get_results();

                    if (output_csv) {
                        db::Formatter::print_csv(headers, rows, std::cout);
                    } else {
                        db::Formatter::print_ascii_table(headers, rows, std::cout);
                    }
                } else {
                    // 其他指令只印成功訊息
                    std::cout << stmt->name() << " executed successfully." << std::endl;
                }
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
            }

            sql.clear();
        } else {
            sql += " ";
        }
    }

    std::cout << "Bye!" << std::endl;
    return 0;
}
