/**
 * @file    logger.cpp
 * @brief   Source file for the Logger class.
 * @details This file contains the implementation of the Logger class which is
 *          responsible for configuring the engine logging behavior through
 *          spdlog.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-02
 */

#include "cbit/core/logger.hpp"

#include <memory>
#include <vector>

#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace cbit2d::core {

/**
 * @brief Initializes the engine logger configuration.
 * @details This sets the output pattern, enables verbose logging, and flushes
 *          automatically on error-level messages.
 */
void Logger::initialize()
{
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/cbit2d.log", true);
    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
    auto logger = std::make_shared<spdlog::logger>("cbit2d", sinks.begin(), sinks.end());

    logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::err);

    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::err);
}

} // namespace cbit2d::core
