#pragma once

#include <iostream>
#include <string>

/**
 * @brief output debug log.
 * @param ... what you want to output.
 * @version 1.0
 * @date 2024/9/19
 * @author ProYRB
 */
#define Debug_Log(...)                                                                             \
    while (Debug && (Debug_Level >= 3))                                                            \
    {                                                                                              \
        std::string temporary(__FILE__);                                                           \
        const std::size_t index = temporary.rfind("/");                                            \
        std::clog << "\033[0;30m" << "["                                                           \
                  << ((string::npos != index) ? temporary.substr(index + 1) : temporary) << ":"    \
                  << __LINE__ << "]" << __VA_ARGS__ << "\033[0m" << std::endl;                     \
        break;                                                                                     \
    }

/**
 * @brief output debug notice.
 * @param ... what you want to output.
 * @version 1.0
 * @date 2024/9/19
 * @author ProYRB
 */
#define Debug_Notice(...)                                                                          \
    while (Debug && (Debug_Level >= 2))                                                            \
    {                                                                                              \
        std::string temporary(__FILE__);                                                           \
        const std::size_t index = temporary.rfind("/");                                            \
        std::clog << "\033[0;32m" << "["                                                           \
                  << ((string::npos != index) ? temporary.substr(index + 1) : temporary) << ":"    \
                  << __LINE__ << "]" << __VA_ARGS__ << "\033[0m" << std::endl;                     \
        break;                                                                                     \
    }

/**
 * @brief output debug warn.
 * @param ... what you want to output.
 * @version 1.0
 * @date 2024/9/19
 * @author ProYRB
 */
#define Debug_Warn(...)                                                                            \
    while (Debug && (Debug_Level >= 1))                                                            \
    {                                                                                              \
        std::string temporary(__FILE__);                                                           \
        const std::size_t index = temporary.rfind("/");                                            \
        std::clog << "\033[1;33m" << "["                                                           \
                  << ((string::npos != index) ? temporary.substr(index + 1) : temporary) << ":"    \
                  << __LINE__ << "]" << __VA_ARGS__ << "\033[0m" << std::endl;                     \
        break;                                                                                     \
    }

/**
 * @brief output debug error.
 * @param ... what you want to output.
 * @version 1.0
 * @date 2024/9/19
 * @author ProYRB
 */
#define Debug_Error(...)                                                                           \
    while (Debug && (Debug_Level >= 0))                                                            \
    {                                                                                              \
        std::string temporary(__FILE__);                                                           \
        const std::size_t index = temporary.rfind("/");                                            \
        std::cerr << "\033[1;31m" << "["                                                           \
                  << ((string::npos != index) ? temporary.substr(index + 1) : temporary) << ":"    \
                  << __LINE__ << "]" << __VA_ARGS__ << "\033[0m" << std::endl;                     \
        break;                                                                                     \
    }
