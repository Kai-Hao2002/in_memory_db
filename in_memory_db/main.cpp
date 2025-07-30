// src/main.cpp
#include <iostream>
#include <string>
#include <memory>
#include "db/Database.hpp"
#include "parser/Parser.hpp"
#include "statement/Statement.hpp"

int main() {
    db::Database database;

    std::cout << "Welcome to the In-Memory DB. Enter SQL commands (type 'exit;' to quit):" << std::endl;

    std::string line;
    std::string sql;

    while (true) {
        std::cout << "SQL> ";
        if (!std::getline(std::cin, line)) {
            break; // EOF or input error
        }

        sql += line;
        // 判斷是否輸入結束（以分號 ; 結尾表示一條完整 SQL）
        if (!sql.empty() && sql.back() == ';') {
            // 移除分號
            sql.pop_back();

            try {
                Parser parser(sql);
                statement::StatementPtr stmt = parser.parse();

                // 執行語句
                stmt->execute(database);

                std::cout << "SQL executed successfully." << std::endl;
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
            }

            // 清空緩存等待下一條命令
            sql.clear();
        } else {
            // 如果沒遇到分號，繼續讀取下一行，支援多行 SQL
            sql += " ";
        }
    }

    std::cout << "Bye!" << std::endl;
    return 0;
}
