//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#pragma once

#ifndef SHARED_H
#define SHARED_H

#include <string>

class Shared {
public:
    static std::string GetExecutablePath();
    static std::string GetUserPath(const std::string& relativePath);
    static bool CopyAndRenameExecutable(const std::string& sourcePath, const std::string& targetPath);
    static bool CreateShortcut(const std::string& targetPath, const std::string& shortcutPath);
};

#endif //SHARED_H
