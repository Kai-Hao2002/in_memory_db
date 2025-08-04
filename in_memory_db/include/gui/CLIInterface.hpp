//include/gui/CLIInterface.hpp
#pragma once
#include <string>
#include "db/Database.hpp"

namespace gui {

class CLIInterface {
public:
    void run();  // 啟動 CLI 主流程
private:
    db::Database database_;
    bool output_csv_ = false;

    std::string to_lower(const std::string& s);
    std::string trim(const std::string& s);
    void handle_command(const std::string& command);
    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename);

};

}
