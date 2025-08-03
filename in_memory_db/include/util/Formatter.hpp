//include/util/Formatter.hpp
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "util/Value.hpp"

namespace db {

class Formatter {
public:
    // 輸出 ASCII 表格
    static void print_ascii_table(const std::vector<std::string>& headers,
                                  const std::vector<std::vector<db::Value>>& rows,
                                  std::ostream& os = std::cout);

    // 輸出 CSV 格式
    static void print_csv(const std::vector<std::string>& headers,
                          const std::vector<std::vector<db::Value>>& rows,
                          std::ostream& os = std::cout);
};

} // namespace util
