//include/util/StringUtil.hpp
#pragma once
#include <string>
#include <algorithm>

namespace db {

inline std::string to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

}
