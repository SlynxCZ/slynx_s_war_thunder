//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    static void Info(const std::string& msg);
    static void Warning(const std::string& msg);
    static void Error(const std::string& msg);
};

#endif //LOGGER_H
