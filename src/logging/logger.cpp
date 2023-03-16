/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/

#include "logging/logger.hpp"


LoggerContext::LoggerContext() = default;

LoggerContext::~LoggerContext() = default;

LoggerContext& LoggerContext::instance()
{
    static LoggerContext context;
    return context;
}

LoggingInterface& LoggerContext::logger()
{
    return _logger;
}
