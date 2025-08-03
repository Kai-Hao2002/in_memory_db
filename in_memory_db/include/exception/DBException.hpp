//include/exception/DBException.hpp
#pragma once
#include <stdexcept>
#include <string>

namespace db {
class DBException : public std::runtime_error {
public:
    explicit DBException(const std::string& msg)
      : std::runtime_error("DBException: " + msg) {}
};
}
