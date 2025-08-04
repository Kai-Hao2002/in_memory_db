//src/gui/CLIInterface.cpp
#include "gui/CLIInterface.hpp"
#include "parser/Parser.hpp"
#include "util/Formatter.hpp"
#include "db/Database.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>


namespace gui {

std::string CLIInterface::to_lower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string CLIInterface::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == std::string::npos || last == std::string::npos) return "";
    return str.substr(first, last - first + 1);
}



void CLIInterface::run() {
    db::Database database;
    bool output_csv = false;
    std::cout << "==== Welcome to the In-Memory DB (CLI UI) ====\n"
          << "Enter SQL statements or use shortcuts:\n"
          << "1. Type exit; to quit\n"
          << "2. Type set output csv; to set output csv\n"
          << "3. Type set output ascii; to set output ascii\n"
          << "4. Type save to filename; to save to filename\n"
          << "5. Type load from filename; to load from filename\n";


    std::string line, sql;

    while (true) {
        std::cout << "SQL> ";
        if (!std::getline(std::cin, line)) break;

        std::string trimmed = trim(line);
        std::string lowered = to_lower(trimmed);

        if (!trimmed.empty() && trimmed.back() == ';') {
            trimmed.pop_back();  // 移除 ;
        }

        if (lowered == "exit") break;
        if (lowered == "set output csv") {
            output_csv_ = true;
            std::cout << "Output format set to CSV.\n";
            continue;
        }
        if (lowered == "set output ascii") {
            output_csv_ = false;
            std::cout << "Output format set to ASCII.\n";
            continue;
        }
        if (lowered.rfind("save to ", 0) == 0) {
            std::string filename = trimmed.substr(8);
            try {
                database.save_to_file(trim(filename));
                std::cout << "Database saved to '" << filename << "' successfully.\n";
            } catch (const std::exception& e) {
                std::cerr << "Failed to save: " << e.what() << "\n";
            }
            continue;
        }
        if (lowered.rfind("load from ", 0) == 0) {
            std::string filename = trimmed.substr(10);
            try {
                database.load_from_file(trim(filename));
                std::cout << "Database loaded from '" << filename << "' successfully.\n";
            } catch (const std::exception& e) {
                std::cerr << "Failed to load: " << e.what() << "\n";
            }
            continue;
        }

        sql += line + " ";
        if (!line.empty() && line.back() == ';') {
            sql.pop_back();
            try {
                Parser parser(sql);
                auto stmt = parser.parse();
                stmt->execute(database_);

                if (stmt->has_results()) {
                    auto headers = stmt->get_columns();
                    auto rows = stmt->get_results();
                    if (output_csv_) {
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
            sql.clear();
        }
    }

    std::cout << "Bye!\n";
}

} // namespace gui
