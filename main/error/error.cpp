#include "error.hpp"
#include "debug.hpp"
#include "main.hpp"

using namespace std;

namespace wifi
{
    error::error(const error_enum type) { this->type = type; }

    error::~error() {}

    const error::error_enum error::get_type() { return this->type; }
} // namespace wifi
