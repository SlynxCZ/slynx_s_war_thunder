//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include <windows.h>
#include <string>
#include <sstream>
#include "Shared.h"
#include "logger/Logger.h"
#include "launcher/LauncherManager.h"
#include "game/GameManager.h"
#include <shlobj.h>

int main() {
    Logger::Info("Slynx's War Thunder Launcher, booting...");

    std::string expectedPath = "C:/Slynx-War-Thunder-Launcher";

    // Zkontroluj, jestli jsme ve správné složce
    if (!Shared::CheckAppDirectory(expectedPath)) {
        Logger::Error("Please run the application from the correct directory.");
        return -1;
    }

    LauncherManager launcher;
    if (launcher.StartLauncher(20)) {
        Logger::Info("Boot finished! Starting game...");

        GameManager game;
        if (!game.StartGame()) {
            Logger::Error("Failed to launch the game.");
        }
    } else {
        Logger::Error("Launcher could not boot.");
    }

    system("pause");
    return 0;
}

std::string Shared::GetExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return std::string(buffer);
}

bool Shared::CheckAppDirectory(const std::string& expectedPath) {
    std::string currentPath = GetExecutablePath();
    size_t pos = currentPath.find_last_of("\\/");
    std::string currentDir = currentPath.substr(0, pos);

    if (currentDir == expectedPath) {
        Logger::Info("Correct directory: " + currentDir);
        return true;
    }
    Logger::Error("Incorrect directory. Expected: " + expectedPath + " but got: " + currentDir);
    return false;
}

bool Shared::CopyAndRenameExecutable(const std::string& sourcePath, const std::string& targetPath) {
    if (CopyFileA(sourcePath.c_str(), targetPath.c_str(), FALSE)) {
        std::stringstream message;
        message << "File copied and renamed successfully, root path of new launcher: " << targetPath;
        Logger::Info(message.str());
        return true;
    }
    std::stringstream message;
    message << "Error copying file: " << GetLastError();
    Logger::Error(message.str());
    return false;
}

std::string Shared::GetUserPath(const std::string& relativePath) {
    char *userProfile = std::getenv("USERPROFILE");
    if (!userProfile) return relativePath;

    return std::string(userProfile) + relativePath;
}

bool Shared::CreateShortcut(const std::string& targetPath, const std::string& shortcutPath) {
    IShellLinkW* pShellLink = nullptr;
    IPersistFile* pPersistFile = nullptr;

    std::wstring targetPathW(targetPath.begin(), targetPath.end());
    std::wstring shortcutPathW(shortcutPath.begin(), shortcutPath.end());

    HRESULT hResult = CoInitialize(nullptr);
    if (FAILED(hResult)) {
        Logger::Error("CoInitialize failed");
        return false;
    }

    hResult = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkW, reinterpret_cast<void**>(&pShellLink));
    if (FAILED(hResult)) {
        Logger::Error("CoCreateInstance failed");
        return false;
    }

    pShellLink->SetPath(targetPathW.c_str());

    hResult = pShellLink->QueryInterface(IID_IPersistFile, reinterpret_cast<void**>(&pPersistFile));
    if (FAILED(hResult)) {
        Logger::Error("QueryInterface failed");
        return false;
    }

    hResult = pPersistFile->Save(shortcutPathW.c_str(), TRUE);
    if (FAILED(hResult)) {
        Logger::Error("Save failed");
        return false;
    }

    pPersistFile->Release();
    pShellLink->Release();
    CoUninitialize();

    std::stringstream message;
    message << "Shortcut created at " << shortcutPath;
    Logger::Info(message.str());
    return true;
}

void Shared::RegenerateFilesAndCreateShortcut(const std::string& targetPath, const std::string& shortcutPath) {
    Logger::Info("Regenerating necessary files...");

    std::string sourcePath = GetExecutablePath();
    if (!CopyAndRenameExecutable(sourcePath, targetPath)) {
        Logger::Error("Failed to regenerate files.");
        return;
    }

    Logger::Info("Files regenerated successfully.");

    if (!CreateShortcut(targetPath, shortcutPath)) {
        Logger::Error("Failed to create shortcut.");
    }
}