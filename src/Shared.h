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
    static std::string GetSteamURL();
    static bool CheckAppDirectory(const std::string& expectedPath);
    static bool CopyAndRenameExecutable(const std::string& sourcePath, const std::string& targetPath);
    static bool CreateShortcut(const std::string& targetPath, const std::string& shortcutPath);
    static bool IsRunningAsAdmin();
    static bool RequestAdminPrivileges();
    static bool DeleteFileIfExists(const std::string& filePath);
    static void RegenerateFilesAndCreateShortcut(const std::string& targetPath, const std::string& shortcutPath);
    static void SaveSteamURL(const std::string& steamURL);
};

#endif //SHARED_H
