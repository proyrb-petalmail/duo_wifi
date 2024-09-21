#pragma once

// #include "debug.hpp"
// #include "main.hpp"

#include <exception>
#include <string>

namespace wifi
{
    class error : public std::exception
    {
    public:
        /**
         * @brief the enum of error.
         * @param Null null error.
         * @param Open open error.
         * @param Read read error.
         * @param Copy copy error.
         * @param Find find error.
         * @param Range range error.
         * @param Create create error.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        typedef enum
        {
            Void = 0UL,
            Open = 1UL << 0,
            Read = 1UL << 1,
            Copy = 1UL << 2,
            Json = 1UL << 3,
            Find = 1UL << 4,
            Type = 1UL << 5,
            Range = 1UL << 6,
            Create = 1UL << 7,
        } error_enum;

    private:
        error_enum type;

    public:
        /**
         * @brief generate a error with type.
         * @param type which type of error.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        error(const error_enum type);

        ~error();

        /**
         * @brief get type of error.
         * @return return the type of error.
         * @version 1.0
         * @date 2024/9/19
         * @author ProYRB
         */
        const error_enum get_type();
    };
} // namespace wifi
