/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <cstdint>


enum class LogLevel : uint8_t
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class LoggingInterface
{};

/**
 * A class designed to hold a logging context for the Pico.
 *
 * @note This class is designed to be a singleton.
 */
class LoggerContext
{
public:
    /**
     * @return The logging context.
     */
    static LoggerContext& instance();

    /**
     * @return The logging interface owned by this context.
     */
    LoggingInterface& logger();

private:
    LoggerContext();
    ~LoggerContext();

    LoggingInterface _logger;
};

/**
 * @return The logging interface for this Pico's runtime.
 */
inline LoggingInterface& logger()
{
    return LoggerContext::instance().logger();
}
