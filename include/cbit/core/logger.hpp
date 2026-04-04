/**
 * @file    logger.hpp
 * @brief   Header file for the Logger class.
 * @details This file contains the definition of the Logger class which is
 *          responsible for routing engine log messages through spdlog.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-02
 */

#pragma once

#include <spdlog/spdlog.h>

#include <utility>

namespace cbit2d::core {

/**
 * @brief Provides the engine logging interface.
 * @details This wrapper keeps spdlog behind the engine API so game code can
 *          log through a stable `cbit2d` interface.
 */
class Logger {
public:
    /**
     * @brief Initializes the logging system.
     */
    static void initialize();

    /**
     * @brief Logs a trace message.
     * @tparam Args Format argument types.
     * @param fmt Format string used by spdlog.
     * @param args Format arguments forwarded to the logger.
     */
    template <typename... Args>
    static void trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::trace(fmt, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a debug message.
     * @tparam Args Format argument types.
     * @param fmt Format string used by spdlog.
     * @param args Format arguments forwarded to the logger.
     */
    template <typename... Args>
    static void debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::debug(fmt, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs an informational message.
     * @tparam Args Format argument types.
     * @param fmt Format string used by spdlog.
     * @param args Format arguments forwarded to the logger.
     */
    template <typename... Args>
    static void info(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::info(fmt, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a warning message.
     * @tparam Args Format argument types.
     * @param fmt Format string used by spdlog.
     * @param args Format arguments forwarded to the logger.
     */
    template <typename... Args>
    static void warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::warn(fmt, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs an error message.
     * @tparam Args Format argument types.
     * @param fmt Format string used by spdlog.
     * @param args Format arguments forwarded to the logger.
     */
    template <typename... Args>
    static void error(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::error(fmt, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a critical message.
     * @tparam Args Format argument types.
     * @param fmt Format string used by spdlog.
     * @param args Format arguments forwarded to the logger.
     */
    template <typename... Args>
    static void critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::critical(fmt, std::forward<Args>(args)...);
    }
};

} // namespace cbit2d::core
