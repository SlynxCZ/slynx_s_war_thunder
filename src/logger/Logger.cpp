//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include "Logger.h"
#include <iostream>
#include <windows.h>

namespace {
    void SetConsoleColor(WORD color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }
}

void Logger::Info(const std::string& msg) {
    SetConsoleColor(7);
    std::cout << "[INFO] " << msg << std::endl;
}

void Logger::Warning(const std::string& msg) {
    SetConsoleColor(14);
    std::cout << "[WARNING] " << msg << std::endl;
}

void Logger::Error(const std::string& msg) {
    SetConsoleColor(12);
    std::cerr << "[ERROR] " << msg << std::endl;
}
