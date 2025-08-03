//main.cpp
#include <iostream>
#include <string>
#include <cctype>
#include <memory>
#include <sstream>
#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Statement.hpp"
#include "util/Formatter.hpp"


std::string to_lower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == std::string::npos || last == std::string::npos) return "";
    return str.substr(first, last - first + 1);
}

int main() {
    db::Database database;
    bool output_csv = false;

    std::cout << "Welcome to the In-Memory DB. Enter SQL commands.\n"
                 "Available Commands:\n"
                 "  1. exit;\n"
                 "  2. set output csv;\n"
                 "  3. set output ascii;\n"
                 "  4. save to filename;\n"
                 "  5. load from filename;\n";

    std::string line;
    std::string sql;

    while (true) {
        std::cout << "SQL> ";
        if (!std::getline(std::cin, line)) break;

        std::string trimmed = trim(line);
        if (!trimmed.empty() && trimmed.back() == ';') {
            trimmed.pop_back(); // 移除尾端 ;
        }

        std::string lowered = to_lower(trimmed);

        // 特殊指令
        if (lowered == "exit") {
            break;
        } else if (lowered == "set output csv") {
            output_csv = true;
            std::cout << "Output format set to CSV.\n";
            continue;
        } else if (lowered == "set output ascii") {
            output_csv = false;
            std::cout << "Output format set to ASCII table.\n";
            continue;
        } else if (lowered.rfind("save to ", 0) == 0) {
            std::string filename = trimmed.substr(8);
            try {
                database.save_to_file(trim(filename));
                std::cout << "Database saved to '" << filename << "' successfully.\n";
            } catch (const std::exception& e) {
                std::cerr << "Failed to save: " << e.what() << "\n";
            }
            continue;
        } else if (lowered.rfind("load from ", 0) == 0) {
            std::string filename = trimmed.substr(10);
            try {
                database.load_from_file(trim(filename));
                std::cout << "Database loaded from '" << filename << "' successfully.\n";
            } catch (const std::exception& e) {
                std::cerr << "Failed to load: " << e.what() << "\n";
            }
            continue;
        }

        // 多行 SQL 組合
        sql += line + " ";
        if (!line.empty() && line.back() == ';') {
            sql.pop_back(); // 移除末尾 ;
            try {
                Parser parser(sql);
                statement::StatementPtr stmt = parser.parse();
                stmt->execute(database);

                if (stmt->has_results()) {
                    auto headers = stmt->get_columns();
                    auto rows = stmt->get_results();
                    if (output_csv) {
                        db::Formatter::print_csv(headers, rows, std::cout);
                    } else {
                        db::Formatter::print_ascii_table(headers, rows, std::cout);
                    }
                } else {
                    std::cout << stmt->name() << " executed successfully.\n";
                }
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << "\n";
            }
            sql.clear(); // 清除無論成功或失敗
        }
    }

    std::cout << "Bye!\n";
    return 0;
}
