//src/util/Formatter.cpp
#include "util/Formatter.hpp"
#include <iomanip>
#include <sstream>


namespace db {

void Formatter::print_ascii_table(const std::vector<std::string>& headers,
                                 const std::vector<std::vector<db::Value>>& rows,
                                 std::ostream& os) {
    // Calculate the maximum width of each column
    std::vector<size_t> col_widths(headers.size(), 0);
    for (size_t i = 0; i < headers.size(); ++i) {
        col_widths[i] = headers[i].size();
    }
    for (const auto& row : rows) {
        for (size_t i = 0; i < headers.size(); ++i) {
            std::ostringstream oss;
            if (i < row.size()) {
                oss << row[i];
            } else {
                oss << "NULL";
            }
            col_widths[i] = std::max(col_widths[i], oss.str().size());
        }
    }

    auto print_line = [&]() {
        os << "+";
        for (auto w : col_widths) {
            os << std::string(w + 2, '-') << "+";
        }
        os << "\n";
    };

    print_line();
    // print headers
    os << "|";
    for (size_t i = 0; i < headers.size(); ++i) {
        os << " " << std::left << std::setw(col_widths[i]) << headers[i] << " |";
    }
    os << "\n";
    print_line();

    // print rows
    for (const auto& row : rows) {
        os << "|";
        for (size_t i = 0; i < headers.size(); ++i) {
            std::ostringstream oss;
            if (i < row.size()) {
                oss << row[i];
            } else {
                oss << "NULL";
            }
            os << " " << std::left << std::setw(col_widths[i]) << oss.str() << " |";
        }
        os << "\n";
    }
    print_line();
}


void Formatter::print_csv(const std::vector<std::string>& headers,
                          const std::vector<std::vector<db::Value>>& rows,
                          std::ostream& os) {
    // headers
    for (size_t i = 0; i < headers.size(); ++i) {
        if (i > 0) os << ",";
        os << "\"" << headers[i] << "\"";
    }
    os << "\n";

    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i > 0) os << ",";
            std::ostringstream oss;
            oss << row[i];
            // Quotes can be added to enhance processing as needed
            os << "\"" << oss.str() << "\"";
        }
        os << "\n";
    }
}

} // namespace util
